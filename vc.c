//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//           INSTITUTO POLITÉCNICO DO CÁVADO E DO AVE
//                          2011/2012
//             ENGENHARIA DE SISTEMAS INFORMÁTICOS
//                    VISÃO POR COMPUTADOR
//
//             [  DUARTE DUQUE - dduque@ipca.pt  ]
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Desabilita (no MSVC++) warnings de funções não seguras (fopen, sscanf, etc...)
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h>
#include "vc.h"


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//            FUNÇÕES: ALOCAR E LIBERTAR UMA IMAGEM
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


// Alocar memória para uma imagem
IVC* vc_image_new(int width, int height, int channels, int levels)
{
	IVC* image = (IVC*)malloc(sizeof(IVC));

	if (image == NULL) return NULL;
	if ((levels <= 0) || (levels > 255)) return NULL;

	image->width = width;
	image->height = height;
	image->channels = channels;
	image->levels = levels;
	image->bytesperline = image->width * image->channels;
	image->data = (unsigned char*)malloc(image->width * image->height * image->channels * sizeof(char));

	if (image->data == NULL)
	{
		return vc_image_free(image);
	}

	return image;
}


// Libertar memória de uma imagem
IVC* vc_image_free(IVC* image)
{
	if (image != NULL)
	{
		if (image->data != NULL)
		{
			free(image->data);
			image->data = NULL;
		}

		free(image);
		image = NULL;
	}

	return image;
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//    FUNÇÕES: LEITURA E ESCRITA DE IMAGENS (PBM, PGM E PPM)
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


char* netpbm_get_token(FILE* file, char* tok, int len)
{
	char* t;
	int c;

	for (;;)
	{
		while (isspace(c = getc(file)));
		if (c != '#') break;
		do c = getc(file);
		while ((c != '\n') && (c != EOF));
		if (c == EOF) break;
	}

	t = tok;

	if (c != EOF)
	{
		do
		{
			*t++ = c;
			c = getc(file);
		} while ((!isspace(c)) && (c != '#') && (c != EOF) && (t - tok < len - 1));

		if (c == '#') ungetc(c, file);
	}

	*t = 0;

	return tok;
}


long int unsigned_char_to_bit(unsigned char* datauchar, unsigned char* databit, int width, int height)
{
	int x, y;
	int countbits;
	long int pos, counttotalbytes;
	unsigned char* p = databit;

	*p = 0;
	countbits = 1;
	counttotalbytes = 0;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = width * y + x;

			if (countbits <= 8)
			{
				// Numa imagem PBM:
				// 1 = Preto
				// 0 = Branco
				//*p |= (datauchar[pos] != 0) << (8 - countbits);

				// Na nossa imagem:
				// 1 = Branco
				// 0 = Preto
				*p |= (datauchar[pos] == 0) << (8 - countbits);

				countbits++;
			}
			if ((countbits > 8) || (x == width - 1))
			{
				p++;
				*p = 0;
				countbits = 1;
				counttotalbytes++;
			}
		}
	}

	return counttotalbytes;
}


void bit_to_unsigned_char(unsigned char* databit, unsigned char* datauchar, int width, int height)
{
	int x, y;
	int countbits;
	long int pos;
	unsigned char* p = databit;

	countbits = 1;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = width * y + x;

			if (countbits <= 8)
			{
				// Numa imagem PBM:
				// 1 = Preto
				// 0 = Branco
				//datauchar[pos] = (*p & (1 << (8 - countbits))) ? 1 : 0;

				// Na nossa imagem:
				// 1 = Branco
				// 0 = Preto
				datauchar[pos] = (*p & (1 << (8 - countbits))) ? 0 : 1;

				countbits++;
			}
			if ((countbits > 8) || (x == width - 1))
			{
				p++;
				countbits = 1;
			}
		}
	}
}


IVC* vc_read_image(char* filename)
{
	FILE* file = NULL;
	IVC* image = NULL;
	unsigned char* tmp;
	char tok[20];
	long int size, sizeofbinarydata;
	int width, height, channels;
	int levels = 255;
	int v;

	// Abre o ficheiro
	if ((file = fopen(filename, "rb")) != NULL)
	{
		// Efectua a leitura do header
		netpbm_get_token(file, tok, sizeof(tok));

		if (strcmp(tok, "P4") == 0) { channels = 1; levels = 1; }	// Se PBM (Binary [0,1])
		else if (strcmp(tok, "P5") == 0) channels = 1;				// Se PGM (Gray [0,MAX(level,255)])
		else if (strcmp(tok, "P6") == 0) channels = 3;				// Se PPM (RGB [0,MAX(level,255)])
		else
		{
#ifdef VC_DEBUG
			printf("ERROR -> vc_read_image():\n\tFile is not a valid PBM, PGM or PPM file.\n\tBad magic number!\n");
#endif

			fclose(file);
			return NULL;
		}

		if (levels == 1) // PBM
		{
			if (sscanf(netpbm_get_token(file, tok, sizeof(tok)), "%d", &width) != 1 ||
				sscanf(netpbm_get_token(file, tok, sizeof(tok)), "%d", &height) != 1)
			{
#ifdef VC_DEBUG
				printf("ERROR -> vc_read_image():\n\tFile is not a valid PBM file.\n\tBad size!\n");
#endif

				fclose(file);
				return NULL;
			}

			// Aloca memória para imagem
			image = vc_image_new(width, height, channels, levels);
			if (image == NULL) return NULL;

			sizeofbinarydata = (image->width / 8 + ((image->width % 8) ? 1 : 0)) * image->height;
			tmp = (unsigned char*)malloc(sizeofbinarydata);
			if (tmp == NULL) return 0;

#ifdef VC_DEBUG
			printf("\nchannels=%d w=%d h=%d levels=%d\n", image->channels, image->width, image->height, levels);
#endif

			if ((v = fread(tmp, sizeof(unsigned char), sizeofbinarydata, file)) != sizeofbinarydata)
			{
#ifdef VC_DEBUG
				printf("ERROR -> vc_read_image():\n\tPremature EOF on file.\n");
#endif

				vc_image_free(image);
				fclose(file);
				free(tmp);
				return NULL;
			}

			bit_to_unsigned_char(tmp, image->data, image->width, image->height);

			free(tmp);
		}
		else // PGM ou PPM
		{
			if (sscanf(netpbm_get_token(file, tok, sizeof(tok)), "%d", &width) != 1 ||
				sscanf(netpbm_get_token(file, tok, sizeof(tok)), "%d", &height) != 1 ||
				sscanf(netpbm_get_token(file, tok, sizeof(tok)), "%d", &levels) != 1 || levels <= 0 || levels > 255)
			{
#ifdef VC_DEBUG
				printf("ERROR -> vc_read_image():\n\tFile is not a valid PGM or PPM file.\n\tBad size!\n");
#endif

				fclose(file);
				return NULL;
			}

			// Aloca memória para imagem
			image = vc_image_new(width, height, channels, levels);
			if (image == NULL) return NULL;

#ifdef VC_DEBUG
			printf("\nchannels=%d w=%d h=%d levels=%d\n", image->channels, image->width, image->height, levels);
#endif

			size = image->width * image->height * image->channels;

			if ((v = fread(image->data, sizeof(unsigned char), size, file)) != size)
			{
#ifdef VC_DEBUG
				printf("ERROR -> vc_read_image():\n\tPremature EOF on file.\n");
#endif

				vc_image_free(image);
				fclose(file);
				return NULL;
			}
		}

		fclose(file);
	}
	else
	{
#ifdef VC_DEBUG
		printf("ERROR -> vc_read_image():\n\tFile not found.\n");
#endif
	}

	return image;
}


int vc_write_image(char* filename, IVC* image)
{
	FILE* file = NULL;
	unsigned char* tmp;
	long int totalbytes, sizeofbinarydata;

	if (image == NULL) return 0;

	if ((file = fopen(filename, "wb")) != NULL)
	{
		if (image->levels == 1)
		{
			sizeofbinarydata = (image->width / 8 + ((image->width % 8) ? 1 : 0)) * image->height + 1;
			tmp = (unsigned char*)malloc(sizeofbinarydata);
			if (tmp == NULL) return 0;

			fprintf(file, "%s %d %d\n", "P4", image->width, image->height);

			totalbytes = unsigned_char_to_bit(image->data, tmp, image->width, image->height);
			printf("Total = %ld\n", totalbytes);
			if (fwrite(tmp, sizeof(unsigned char), totalbytes, file) != totalbytes)
			{
#ifdef VC_DEBUG
				fprintf(stderr, "ERROR -> vc_read_image():\n\tError writing PBM, PGM or PPM file.\n");
#endif

				fclose(file);
				free(tmp);
				return 0;
			}

			free(tmp);
		}
		else
		{
			fprintf(file, "%s %d %d 255\n", (image->channels == 1) ? "P5" : "P6", image->width, image->height);

			if (fwrite(image->data, image->bytesperline, image->height, file) != image->height)
			{
#ifdef VC_DEBUG
				fprintf(stderr, "ERROR -> vc_read_image():\n\tError writing PBM, PGM or PPM file.\n");
#endif

				fclose(file);
				return 0;
			}
		}

		fclose(file);

		return 1;
	}

	return 0;
}


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//            FUNÇÕES: desenvolvidas nas AULAS
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//Calcula o negativo de uma imagem GREY
int vc_gray_negative(IVC* srcdst)
{
	//variaveis necessarias para constituir 1 imagem
	unsigned char* data = (unsigned char*)srcdst->data;
	int with = srcdst->width;
	int height = srcdst->height;
	int bytesperline = srcdst->width * srcdst->channels;
	int channels = srcdst->channels;
	int x, y;
	long int pos;

	//verifica erros
	if (srcdst == NULL)
	{
		printf("ERROR -> vc_gray_negative():\n\tImage is empty!\n");
		getchar();
		return 0;
	}

	if (srcdst->width <= 0 || srcdst->height <= 0 || srcdst->data == NULL)
	{
		printf("ERROR -> vc_gray_negative():\n\tDimensoes or data are missing!\n");
		getchar();
		return 0;
	}

	if (channels != 1)
	{
		printf("ERROR -> vc_gray_negative():\n\tNot Gray Image!\n");
		getchar();
		return 0;
	}

	//inverte a imagem GRAY
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < with; x++)
		{
			pos = y * bytesperline + x * channels;

			data[pos] = 255 - data[pos];
		}
	}

	return 1;
}

//Calcula o negativo de uma imagem RGB
int vc_rgb_negative(IVC* srcdst)
{

	unsigned char* data = (unsigned char*)srcdst->data;
	int width = srcdst->width;
	int height = srcdst->height;
	int bytesperline = srcdst->width * srcdst->channels;
	int channels = srcdst->channels;
	int x, y;
	long int pos;

	//verificar erros
	if (srcdst == NULL)
	{
		printf("ERROR -> vc_rgb_negative():\n\tImage is empty!\n");
		getchar();
		return 0;
	}

	if (srcdst->width <= 0 || srcdst->height <= 0 || srcdst->data == NULL)
	{
		printf("ERROR -> vc_rgb_negative():\n\tDimensoes or data are missing!\n");
		getchar();
		return 0;
	}

	if (channels != 3)
	{
		printf("ERROR -> vc_rgb_negative():\n\tNot Color Image!\n");
		getchar();
		return 0;
	}

	//inverte a imagem RGB
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;

			data[pos] = 255 - data[pos];
			data[pos + 1] = 255 - data[pos + 1];
			data[pos + 2] = 255 - data[pos + 2];
		}
	}

	return 1;
}

//Removes the color RED from a RGB picture
int vc_rgb_get_red_gray(IVC* srcdst) {
	unsigned char* data = (unsigned char*)srcdst->data;
	int width = srcdst->width;
	int height = srcdst->height;
	int bytesperline = srcdst->width * srcdst->channels;
	int channels = srcdst->channels;
	int x, y;
	long int pos;

	//verificar erros
	if (srcdst == NULL)
	{
		printf("ERROR -> vc_rgb_get_red_gray():\n\tImage is empty!\n");
		getchar();
		return 0;
	}

	if (srcdst->width <= 0 || srcdst->height <= 0 || srcdst->data == NULL)
	{
		printf("ERROR -> vc_rgb_get_red_gray():\n\tDimensoes or data are missing!\n");
		getchar();
		return 0;
	}

	if (channels != 3)
	{
		printf("ERROR -> vc_rgb_get_red_gray():\n\tNot Color Image!\n");
		getchar();
		return 0;
	}

	//Removes RED
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;

			data[pos + 1] = data[pos];
			data[pos + 2] = data[pos];
		}
	}

	return 1;
}

//Removes the color GREEN from a RGB picture
int vc_rgb_get_green_gray(IVC* srcdst) {
	unsigned char* data = (unsigned char*)srcdst->data;
	int width = srcdst->width;
	int height = srcdst->height;
	int bytesperline = srcdst->width * srcdst->channels;
	int channels = srcdst->channels;
	int x, y;
	long int pos;

	//verificar erros
	if (srcdst == NULL)
	{
		printf("ERROR -> vc_rgb_get_green_gray():\n\tImage is empty!\n");
		getchar();
		return 0;
	}

	if (srcdst->width <= 0 || srcdst->height <= 0 || srcdst->data == NULL)
	{
		printf("ERROR -> vc_rgb_get_green_gray():\n\tDimensoes or data are missing!\n");
		getchar();
		return 0;
	}

	if (channels != 3)
	{
		printf("ERROR -> vc_rgb_get_green_gray():\n\tNot Color Image!\n");
		getchar();
		return 0;
	}

	//Removes GREEN
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;

			data[pos] = data[pos + 1];
			data[pos + 2] = data[pos + 1];
		}
	}

	return 1;
}

//Removes the color BLUE from a RGB picture
int vc_rgb_get_blue_gray(IVC* srcdst) {
	unsigned char* data = (unsigned char*)srcdst->data;
	int width = srcdst->width;
	int height = srcdst->height;
	int bytesperline = srcdst->width * srcdst->channels;
	int channels = srcdst->channels;
	int x, y;
	long int pos;

	//verificar erros
	if (srcdst == NULL)
	{
		printf("ERROR -> vc_rgb_get_blue_gray():\n\tImage is empty!\n");
		getchar();
		return 0;
	}

	if (srcdst->width <= 0 || srcdst->height <= 0 || srcdst->data == NULL)
	{
		printf("ERROR -> vc_rgb_get_blue_gray():\n\tDimensoes or data are missing!\n");
		getchar();
		return 0;
	}

	if (channels != 3)
	{
		printf("ERROR -> vc_rgb_get_blue_gray():\n\tNot Color Image!\n");
		getchar();
		return 0;
	}

	//Removes BLUE
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;

			data[pos] = data[pos + 2];
			data[pos + 1] = data[pos + 2];
		}
	}

	return 1;
}

//IN goes a RGB pic and OUT comes a GRAY pic
int vc_rgb_to_gray(IVC* src, IVC* dst) {
	unsigned char* datasrc = (unsigned char*)src->data;
	int bytesperlin_src = src->width * src->channels;
	int channels_src = src->channels;
	unsigned char* datadst = (unsigned char*)dst->data;
	int bytesperlin_dst = dst->width * dst->channels;
	int channels_dst = dst->channels;
	int width = src->width;
	int height = src->height;
	long int pos_src, pos_dst;
	float rf, gf, bf;
	int x, y;

	//verificar erros
	if (src == NULL)
	{
		printf("ERROR -> vc_rgb_to_gray():\n\tImage is empty!\n");
		getchar();
		return 0;
	}

	if (src->width <= 0 || src->height <= 0 || src->data == NULL)
	{
		printf("ERROR -> vc_rgb_to_gray():\n\tDimensoes or data are missing!\n");
		getchar();
		return 0;
	}

	if (src->channels != 3 || dst->channels != 1)
	{
		printf("ERROR -> vc_rgb_to_gray():\n\tNot Color Image!\n");
		getchar();
		return 0;
	}

	//SET RGB to GRAY
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos_src = y * bytesperlin_src + x * channels_src;
			pos_dst = y * bytesperlin_dst + x * channels_dst;

			rf = (float)datasrc[pos_src];
			gf = (float)datasrc[pos_src + 1];
			bf = (float)datasrc[pos_src + 2];

			//Calculo da media para GRAY
			datadst[pos_dst] = (unsigned)rf * 0.299 + gf * 0.587 + bf * 0.114;
		}
	}

	return 1;

}

//Converts color RGB to HSV
int vc_rgb_to_hsv(IVC* srcdst) {
	unsigned char* data = (unsigned char*)srcdst->data;
	int width = srcdst->width;
	int height = srcdst->height;
	int bytesperline = srcdst->bytesperline;
	int channels = srcdst->channels;
	float r, g, b, hue, saturation, value;
	float rgb_max, rgb_min;
	int i, size;

	//verificar erros
	if (srcdst == NULL)
	{
		printf("ERROR -> vc_rgb_to_hsv():\n\tImage is empty!\n");
		getchar();
		return 0;
	}

	if (srcdst->width <= 0 || srcdst->height <= 0 || srcdst->data == NULL)
	{
		printf("ERROR -> vc_rgb_to_hsv():\n\tDimensoes or data are missing!\n");
		getchar();
		return 0;
	}

	if (channels != 3)
	{
		printf("ERROR -> vc_rgb_to_hsv():\n\tNot Color Image!\n");
		getchar();
		return 0;
	}

	size = width * height * channels;

	for (i = 0; i < size; i = i + channels)
	{
		r = (float)data[i];
		g = (float)data[i + 1];
		b = (float)data[i + 2];

		//// Calcula o MAX de RGB
			//if (r >= g) {
			//	if (r >= b)
			//		rgb_max = r;
			//	else
			//		rgb_max = b;
			//}
			//else {
			//	if (g >= b)
			//		rgb_max = g;
			//	else
			//		rgb_max = b;
			//}

			//// Calcula o MIN de RGB
			//if (r <= g) {
			//	if (r <= b)
			//		rgb_min = r;
			//	else
			//		rgb_min = b;
			//}
			//else {
			//	if (g <= b)
			//		rgb_min = g;
			//	else
			//		rgb_min = b;
			//}

		// Calcula valores máximo e mínimo dos canais de cor R, G e B
		rgb_max = (r > g ? (r > b ? r : b) : (g > b ? g : b));
		rgb_min = (r < g ? (r < b ? r : b) : (g < b ? g : b));

		// Value toma valores entre [0,255]
		value = rgb_max;
		if (value == 0.0f)
		{
			hue = 0.0f;
			saturation = 0.0f;
		}
		else
		{
			// Saturation toma valores entre [0,255]
			saturation = ((rgb_max - rgb_min) / rgb_max) * 255.0f;

			if (saturation == 0.0f)
			{
				hue = 0.0f;
			}
			else
			{
				// Hue toma valores entre [0,360]
				if ((rgb_max == r) && (g >= b))
				{
					hue = 60.0f * (g - b) / (rgb_max - rgb_min);
				}
				else if ((rgb_max == r) && (b > g))
				{
					hue = 360.0f + 60.0f * (g - b) / (rgb_max - rgb_min);
				}
				else if (rgb_max == g)
				{
					hue = 120.0f + 60.0f * (b - r) / (rgb_max - rgb_min);
				}
				else /* rgb_max == b*/
				{
					hue = 240.0f + 60.0f * (r - g) / (rgb_max - rgb_min);
				}
			}
		}

		// Atribui valores entre [0,255]
		data[i] = (unsigned char)(hue / 360.0f * 255.0f);
		data[i + 1] = (unsigned char)(saturation);
		data[i + 2] = (unsigned char)(value);
	}

	return 1;
}

// Se encontrar a cor fica branco tudo o resto fica preto. ver colorpicker no google para escolher os intervalos
// hmin,hmax = [0, 360]; smin,smax = [0, 100]; vmin,vmax = [0, 100]
int vc_hsv_segmentation(IVC* srcdst, int hmin, int hmax, int smin, int smax, int vmin, int vmax) {
	unsigned char* data = (unsigned char*)srcdst->data;
	int width = srcdst->width;
	int height = srcdst->height;
	int bytesperline = srcdst->bytesperline;
	int channels = srcdst->channels;
	int h, s, v; // h=[0, 360] s=[0, 100] v=[0, 100]
	int i, size;

	//verificar erros
	if (srcdst == NULL)
	{
		printf("ERROR -> vc_hsv_segmentation():\n\tImage is empty!\n");
		getchar();
		return 0;
	}

	if (srcdst->width <= 0 || srcdst->height <= 0 || srcdst->data == NULL)
	{
		printf("ERROR -> vc_hsv_segmentation():\n\tDimensoes or data are missing!\n");
		getchar();
		return 0;
	}

	if (channels != 3)
	{
		printf("ERROR -> vc_hsv_segmentation():\n\tNot Color Image!\n");
		getchar();
		return 0;
	}

	//Size == pos [ver funcoes anteriores]
	size = width * height * channels;

	//Se encontrar a cor fica branco else preto
	for (i = 0; i < size; i = i + channels)
	{
		h = ((float)data[i]) / 255.0f * 360.0f;
		s = ((float)data[i + 1]) / 255.0f * 100.0f;
		v = ((float)data[i + 2]) / 255.0f * 100.0f;

		if ((h >= hmin) && (h <= hmax) && (s >= smin) && (s <= smax) && (v >= vmin) && (v <= vmax))
		{
			data[i] = 255;
			data[i + 1] = 255;
			data[i + 2] = 255;
		}
		else
		{
			data[i] = 0;
			data[i + 1] = 0;
			data[i + 2] = 0;
		}
	}

	return 1;
}

//De uma imagem em cinza para uma imagem RGB
int vc_scale_gray_to_rgb(IVC* src, IVC* dst) {
	unsigned char* datasrc = (unsigned char*)src->data;
	int bytesperlin_src = src->width * src->channels;
	int channels_src = src->channels;
	unsigned char* datadst = (unsigned char*)dst->data;
	int bytesperlin_dst = dst->width * dst->channels;
	int channels_dst = dst->channels;
	int width = src->width;
	int height = src->height;
	long int pos_src, pos_dst;
	int x, y;

	//verificar erros
	if (src == NULL)
	{
		printf("ERROR -> vc_scale_gray_to_rgb():\n\tImage is empty!\n");
		getchar();
		return 0;
	}

	if (src->width <= 0 || src->height <= 0 || src->data == NULL)
	{
		printf("ERROR -> vc_scale_gray_to_rgb():\n\tDimensoes or data are missing!\n");
		getchar();
		return 0;
	}

	if (src->channels != 1 || dst->channels != 3)
	{
		printf("ERROR -> vc_scale_gray_to_rgb():\n\tNot Color Image!\n");
		getchar();
		return 0;
	}

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos_src = y * bytesperlin_src + x * channels_src;
			pos_dst = y * bytesperlin_dst + x * channels_dst;

			//Blue to Cyan
			if (datasrc[pos_src] < 63)
			{
				datadst[pos_dst] = 0;
				datadst[pos_dst + 1] = datasrc[pos_src] * 4;
				datadst[pos_dst + 2] = 255;
			}
			//Cyan to Green
			else if (datasrc[pos_src] < 128)
			{
				datadst[pos_dst] = 0;
				datadst[pos_dst + 1] = 255;
				datadst[pos_dst + 2] = 255 - (datasrc[pos_src] - 64) * 4;
			}
			//Green to Yellow
			else if (datasrc[pos_src] < 192)
			{
				datadst[pos_dst] = (datasrc[pos_src] - 128) * 4;
				datadst[pos_dst + 1] = 255;
				datadst[pos_dst + 2] = 0;
			}
			//Yellow to Red
			else
			{
				datadst[pos_dst] = 255;
				datadst[pos_dst + 1] = 255 - (datasrc[pos_src] - 192) * 4;
				datadst[pos_dst + 2] = 0;
			}
		}
	}
	return 1;
}

//FUNÇÃO DE BINARIZAÇÃO com threshold manual
int vc_gray_to_binary(IVC* srcdst, int threshold)
{
	unsigned char* data = (unsigned char*)srcdst->data;
	int width = srcdst->width;
	int height = srcdst->height;
	int bytesperline = srcdst->width * srcdst->channels;
	int channels = srcdst->channels;
	int x, y;
	long int pos;

	if ((width <= 0) || (height <= 0) || (srcdst->data == NULL)) return 0;
	if (channels != 1) return 0;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;
			if (data[pos] < threshold)
			{
				data[pos] = 0;
			}
			else
			{
				data[pos] = 255;
			}
		}
	}
	return 1;
}

//FUNÇÃO DE BINARIZAÇÃO com threshold calculado
int vc_gray_to_binary_global_mean(IVC* srcdst)
{
	int width = srcdst->width;
	int height = srcdst->height;
	int canais = srcdst->channels;
	int x;
	int tamanho, somatorio = 0;
	double media;

	tamanho = width * height * canais;

	for (x = 0; x < tamanho; x++)
	{
		somatorio += srcdst->data[x];
	}

	media = (double)somatorio / tamanho;

	for (x = 0; x < tamanho; x++)
	{
		if (srcdst->data[x] > media)
		{
			srcdst->data[x] = 255;
		}

		else
		{
			srcdst->data[x] = 0;
		}
	}

	return 1;

}

//Use prime numbers to kernel
int vc_gray_to_binary_midpoint(IVC* src, IVC* dst, int kernel)
{
	unsigned char* datasrc = (unsigned char*)src->data;
	unsigned char* datadst = (unsigned char*)dst->data;
	int width = src->width;
	int height = src->height;
	int bytesperline = src->bytesperline;
	int channels = src->channels;
	int x, y, xx, yy;
	int offset = (kernel - 1) / 2;
	int max, min;
	long int pos, posk;
	unsigned char threshold;


	//verificar erros
	if (src == NULL)
	{
		printf("ERROR -> vc_gray_to_binary_midpoint():\n\tImage is empty!\n");
		getchar();
		return 0;
	}

	if (src->width <= 0 || src->height <= 0 || src->data == NULL)
	{
		printf("ERROR -> vc_gray_to_binary_midpoint():\n\tDimensoes or data are missing!\n");
		getchar();
		return 0;
	}

	if (src->channels != 1 || dst->channels != 1)
	{
		printf("ERROR -> vc_gray_to_binary_midpoint():\n\tNot Gray Image!\n");
		getchar();
		return 0;
	}

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;

			max = 0;
			min = 255;

			// NxM Vizinhos
			for (yy = -offset; yy <= offset; yy++)
			{
				for (xx = -offset; xx <= offset; xx++)
				{
					if ((y + yy >= 0) && (y + yy < height) && (x + xx >= 0) && (x + xx < width))
					{
						posk = (y + yy) * bytesperline + (x + xx) * channels;

						if (datasrc[posk] > max) max = datasrc[posk];
						if (datasrc[posk] < min) min = datasrc[posk];
					}
				}
			}

			threshold = (unsigned char)((float)(max + min) / (float)2);

			if (datasrc[pos] > threshold) datadst[pos] = 255;
			else datadst[pos] = 0;
		}
	}

	return 1;
}

//MUITO LENTO E NAO FUNCIONA DIREITO!!!!
int vc_gray_to_binary_niblack(IVC* src, IVC* dst, int kernel, float alfa)
{
	unsigned char* datasrc = (unsigned char*)src->data;
	unsigned char* datadst = (unsigned char*)dst->data;
	int width = src->width;
	int height = src->height;
	int bytesperline = src->bytesperline;
	int channels = src->channels;
	int x, y, xx, yy;
	int offset = (kernel - 1) / 2;
	float mean, std, sum, numberOfPixels;
	long int pos, posk;
	unsigned char threshold;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;

			sum = 0.0;
			mean = 0.0;
			std = 0.0;
			numberOfPixels = 0.0;

			for (yy = -offset; yy <= offset; yy++)
			{
				for (xx = -offset; xx <= offset; xx++)
				{
					if ((y + yy >= 0) && (y + yy < height) && (x + xx >= 0) && (x + xx < width))
					{
						posk = (y + yy) * bytesperline + (x + xx) * channels;

						sum += (float)src->data[posk];

						numberOfPixels += 1;
					}
				}
			}

			mean = sum / (float)(numberOfPixels);

			//std
			for (yy = -offset; yy <= offset; yy++)
			{
				for (xx = -offset; xx <= offset; xx++)
				{
					if ((y + yy >= 0) && (y + yy < height) && (x + xx >= 0) && (x + xx < width))
					{
						posk = (y + yy) * bytesperline + (x + xx) * channels;

						std += pow(((float)src->data[posk] - mean), 2.0);
					}
				}
			}

			std = std / (float)(numberOfPixels);
			std = pow(std, 0.5);

			//theshold
			threshold = (unsigned char)(mean + std * alfa);

			if (src->data[pos] > threshold)
				dst->data[pos] = 255;
			else
				dst->data[pos] = 0;
		}
	}

	return 1;
}

// FUNÇÃO DILATAÇÃO
int vc_binary_dilate(IVC* src, IVC* dst, int kernel)
{
	unsigned char* datasrc = (unsigned char*)src->data;
	unsigned char* datadst = (unsigned char*)dst->data;
	int width = src->width;
	int height = src->height;
	int bytesperline = src->bytesperline;
	int channels = src->channels;
	int x, y, xx, yy;
	int offset = (kernel - 1) / 2;
	int max, min;
	long int pos, posk;
	int aux = 0;

	//verificar erros
	if (src == NULL)
	{
		printf("ERROR -> vc_binary_dilate():\n\tImage is empty!\n");
		getchar();
		return 0;
	}

	if (src->width <= 0 || src->height <= 0 || src->data == NULL)
	{
		printf("ERROR -> vc_binary_dilate():\n\tDimensoes or data are missing!\n");
		getchar();
		return 0;
	}

	if (src->channels != 1 || dst->channels != 1)
	{
		printf("ERROR -> vc_binary_dilate():\n\tNot Gray Image!\n");
		getchar();
		return 0;
	}

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;

			max = datasrc[pos];
			min = datasrc[pos];
			aux = 0;
			// NxM Vizinhos
			for (yy = -offset; yy <= offset; yy++)
			{
				for (xx = -offset; xx <= offset; xx++)
				{
					if ((y + yy >= 0) && (y + yy < height) && (x + xx >= 0) && (x + xx < width))
					{
						posk = (y + yy) * bytesperline + (x + xx) * channels;

						if (datasrc[posk] == 255)
						{
							aux = 255;
						}
					}
				}
			}

			datadst[pos] = aux;


		}
	}

	return 1;
}

//FUNÇÃO EROSÃO
int vc_binary_erode(IVC* src, IVC* dst, int kernel)
{
	unsigned char* datasrc = (unsigned char*)src->data;
	unsigned char* datadst = (unsigned char*)dst->data;
	int width = src->width;
	int height = src->height;
	int bytesperline = src->bytesperline;
	int channels = src->channels;
	int x, y, xx, yy;
	int offset = (kernel - 1) / 2;
	int max, min;
	long int pos, posk;
	int aux = 255;

	//verificar erros
	if (src == NULL)
	{
		printf("ERROR -> vc_binary_erode():\n\tImage is empty!\n");
		getchar();
		return 0;
	}

	if (src->width <= 0 || src->height <= 0 || src->data == NULL)
	{
		printf("ERROR -> vc_binary_erode():\n\tDimensoes or data are missing!\n");
		getchar();
		return 0;
	}

	if (src->channels != 1 || dst->channels != 1)
	{
		printf("ERROR -> vc_binary_erode():\n\tNot Gray Image!\n");
		getchar();
		return 0;
	}

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;

			max = datasrc[pos];
			min = datasrc[pos];
			aux = 255;
			// NxM Vizinhos
			for (yy = -offset; yy <= offset; yy++)
			{
				for (xx = -offset; xx <= offset; xx++)
				{
					if ((y + yy >= 0) && (y + yy < height) && (x + xx >= 0) && (x + xx < width))
					{
						posk = (y + yy) * bytesperline + (x + xx) * channels;

						if (datasrc[posk] == 0)
						{
							aux = 0;
						}
					}
				}
			}

			datadst[pos] = aux;


		}
	}

	return 1;
}

//FUNÇÃO ABERTURA
int vc_binary_open(IVC* src, IVC* dst, int kernel)
{
	//Verificação de erros realizada nas funcoes invocadas abaixo

	IVC* aux = vc_image_new(src->width, src->height, 1, src->levels);
	vc_binary_erode(src, aux, kernel);
	vc_binary_dilate(aux, dst, kernel);
	vc_image_free(aux);

	return 1;
}

//FUNÇÃO FECHO
int vc_binary_close(IVC* src, IVC* dst, int kernel)
{
	//Verificação de erros realizada nas funcoes invocadas abaixo

	IVC* aux = vc_image_new(src->width, src->height, 1, src->levels);
	vc_binary_dilate(src, aux, kernel);
	vc_binary_erode(aux, dst, kernel);
	vc_image_free(aux);

	return 1;
}
