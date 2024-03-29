//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//           INSTITUTO POLIT�CNICO DO C�VADO E DO AVE
//                          2011/2012
//             ENGENHARIA DE SISTEMAS INFORM�TICOS
//                    VIS�O POR COMPUTADOR
//
//             [  DUARTE DUQUE - dduque@ipca.pt  ]
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Desabilita (no MSVC++) warnings de fun��es n�o seguras (fopen, sscanf, etc...)
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include "vc.h"


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//            FUN��ES: ALOCAR E LIBERTAR UMA IMAGEM
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


// Alocar mem�ria para uma imagem
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


// Libertar mem�ria de uma imagem
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
//    FUN��ES: LEITURA E ESCRITA DE IMAGENS (PBM, PGM E PPM)
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

			// Aloca mem�ria para imagem
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

			// Aloca mem�ria para imagem
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
//            FUN��ES: desenvolvidas nas AULAS
// 
//		 [  Pedro Moreira - pedrogmoreria94@gmail.com  ]
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

		// Calcula valores m�ximo e m�nimo dos canais de cor R, G e B
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

//FUN��O DE BINARIZA��O com threshold manual
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

//FUN��O DE BINARIZA��O com threshold calculado
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

// FUN��O DILATA��O
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

//FUN��O EROS�O
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

//FUN��O ABERTURA
int vc_binary_open(IVC* src, IVC* dst, int kernel)
{
	//Verifica��o de erros realizada nas funcoes invocadas abaixo

	IVC* aux = vc_image_new(src->width, src->height, 1, src->levels);
	vc_binary_erode(src, aux, kernel);
	vc_binary_dilate(aux, dst, kernel);
	vc_image_free(aux);

	return 1;
}

//FUN��O FECHO
int vc_binary_close(IVC* src, IVC* dst, int kernel)
{
	//Verifica��o de erros realizada nas funcoes invocadas abaixo

	IVC* aux = vc_image_new(src->width, src->height, 1, src->levels);
	vc_binary_dilate(src, aux, kernel);
	vc_binary_erode(aux, dst, kernel);
	vc_image_free(aux);

	return 1;
}

//FUN��O C�LCULO DO HISTOGRAMA
int vc_gray_histogram(IVC* src, IVC* dst)
{
	unsigned char* data = (unsigned char*)src->data;
	int width = src->width, height = src->height;
	int channels = src->channels;
	int levels = src->levels;
	int bytesperline = src->bytesperline;
	long int pos, size = src->width * src->height;
	int x, y, i;
	int contaPixeis[256] = { 0 };
	float pdf[256];
	float cdf[256] = { 0 };

	//verificar erros
	if (src == NULL)
	{
		printf("ERROR -> vc_gray_histogram():\n\tImage is empty!\n");
		getchar();
		return 0;
	}

	if (src->width <= 0 || src->height <= 0 || src->data == NULL)
	{
		printf("ERROR -> vc_gray_histogram():\n\tDimensoes or data are missing!\n");
		getchar();
		return 0;
	}

	if (src->channels != 1 || dst->channels != 1)
	{
		printf("ERROR -> vc_gray_histogram():\n\tNot Gray Image!\n");
		getchar();
		return 0;
	}

	//tons de cinzento da imagem
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;
			contaPixeis[(int)src->data[pos]]++;
		}
	}

	float pdfmax = 0;
	for (i = 0; i < 256; i++)
	{
		pdf[i] = (float)((float)contaPixeis[i] / (float)(size));
		if (pdf[i] > pdfmax) pdfmax = pdf[i];
	}

	double temp;
	for (x = 0; x < dst->width; x++)
	{
		temp = (float)((float)pdf[x] * (float)255 / pdfmax);
		for (int y = dst->height - 1; y >= 0; y--)
		{
			pos = y * dst->bytesperline + x * dst->channels;
			if ((double)y > 255 - temp)
				dst->data[pos] = (unsigned char)0;
			else
				dst->data[pos] = (unsigned char)255;
		}
	}

	return 1;
}

//FUN��O C�LCULO DO HISTOGRAMA
int vc_gray_histogram_equalization(IVC* src, IVC* dst)
{
	unsigned char* data = (unsigned char*)src->data;
	int width = src->width, height = src->height;
	int channels = src->channels;
	int levels = src->levels;
	int bytesperline = src->bytesperline;
	long int pos;
	long int size = src->width * src->height;
	int contaPixeis[256] = { 0 };
	float pdf[256];
	float cdf[256];
	int i;

	//verificar erros
	if (src == NULL)
	{
		printf("ERROR -> vc_gray_histogram_equalization():\n\tImage is empty!\n");
		getchar();
		return 0;
	}

	if (src->width <= 0 || src->height <= 0 || src->data == NULL)
	{
		printf("ERROR -> vc_gray_histogram_equalization():\n\tDimensoes or data are missing!\n");
		getchar();
		return 0;
	}


	for (size_t y = 0; y < height; y++)
	{
		for (size_t x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;
			contaPixeis[(int)src->data[pos]]++;
		}
	}
	float pdfmax = 0;
	for (size_t i = 0; i < 256; i++)
	{
		pdf[i] = (float)((float)contaPixeis[i] / (float)(size));
		if (pdf[i] > pdfmax) pdfmax = pdf[i];
	}
	double temp;
	for (size_t x = 0; x < dst->width; x++)
	{
		temp = (float)((float)pdf[x] * (float)255 / pdfmax);
		for (int y = dst->height - 1; y >= 0; y--)
		{
			pos = y * dst->bytesperline + x * dst->channels;
			if ((double)y > 255 - temp)
				dst->data[pos] = (unsigned char)0;
			else
				dst->data[pos] = (unsigned char)255;
		}
	}
	//CALCULA O GR�FICO CDF

	for (i = 1, cdf[0] = pdf[0]; i < 256; i++)
	{
		cdf[i] = cdf[i - 1] + pdf[i];
	}


	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;
			dst->data[pos] = cdf[data[pos]] * levels;
		}
	}
	return 1;
}

//Dete��o de contornos Prewitt
int vc_gray_edge_prewitt(IVC* src, IVC* dst, float th)
{
	unsigned char* datasrc = (unsigned char*)src->data;
	unsigned char* datadst = (unsigned char*)dst->data;
	int width = src->width;
	int height = src->height;
	int bytesperline = src->bytesperline;
	int channels = src->channels;
	int x, y;
	long int posX, posA, posB, posC, posD, posE, posF, posG, posH;
	int i, size;
	int histmax, histthreshold;
	int sumx, sumy;
	int hist[256] = { 0 };

	//verificar erros
	if (src == NULL)
	{
		printf("ERROR -> vc_gray_edge_prewitt():\n\tImage is empty!\n");
		getchar();
		return 0;
	}

	if (src->width <= 0 || src->height <= 0 || src->data == NULL)
	{
		printf("ERROR -> vc_gray_edge_prewitt():\n\tDimensoes or data are missing!\n");
		getchar();
		return 0;
	}


	size = width * height;

	for (y = 1; y < height - 1; y++)
	{
		for (x = 1; x < width - 1; x++)
		{
			posA = (y - 1) * bytesperline + (x - 1);
			posB = (y - 1) * bytesperline + x;
			posC = (y - 1) * bytesperline + (x + 1);
			posD = y * bytesperline + (x - 1);
			posX = y * bytesperline + x;
			posE = y * bytesperline + (x + 1);
			posF = (y + 1) * bytesperline + (x - 1);
			posG = (y + 1) * bytesperline + x;
			posH = (y + 1) * bytesperline + (x + 1);

			sumx = datasrc[posA] * -1;
			sumx += datasrc[posD] * -1;
			sumx += datasrc[posF] * -1;

			sumx += datasrc[posC] * +1;
			sumx += datasrc[posE] * +1;
			sumx += datasrc[posH] * +1;
			sumx = sumx / 3; // 3 = 1 + 1 + 1

			sumy = datasrc[posA] * -1;
			sumy += datasrc[posB] * -1;
			sumy += datasrc[posC] * -1;

			sumy += datasrc[posF] * +1;
			sumy += datasrc[posG] * +1;
			sumy += datasrc[posH] * +1;
			sumy = sumy / 3; // 3 = 1 + 1 + 1

			datadst[posX] = (unsigned char)sqrt((double)(sumx * sumx + sumy * sumy));
		}
	}

	// Compute a grey level histogram
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			hist[datadst[y * bytesperline + x * channels]]++;
		}
	}

	// Threshold at the middle of the occupied levels
	histmax = 0;
	for (i = 0; i <= 255; i++)
	{
		histmax += hist[i];

		// th = Prewitt Threshold
		if (histmax >= (((float)size) * th)) break;
	}
	histthreshold = i;


	// Apply the threshold
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			posX = y * bytesperline + x * channels;

			if (datadst[posX] >= histthreshold) datadst[posX] = 255;
			else datadst[posX] = 0;
		}
	}

	return 1;
}

//Dete��o de contornos Sobel
int vc_gray_edge_sobel(IVC* src, IVC* dst, float th)
{
	unsigned char* datasrc = (unsigned char*)src->data;
	unsigned char* datadst = (unsigned char*)dst->data;
	int width = src->width;
	int height = src->height;
	int bytesperline = src->bytesperline;
	int channels = src->channels;
	int x, y;
	long int posX, posA, posB, posC, posD, posE, posF, posG, posH;
	int i, size;
	int histmax, histthreshold;
	int sumx, sumy;
	int hist[256] = { 0 };

	//verificar erros
	if (src == NULL)
	{
		printf("ERROR -> vc_gray_edge_sobel():\n\tImage is empty!\n");
		getchar();
		return 0;
	}

	if (src->width <= 0 || src->height <= 0 || src->data == NULL)
	{
		printf("ERROR -> vc_gray_edge_sobel():\n\tDimensoes or data are missing!\n");
		getchar();
		return 0;
	}

	if (src->channels != 1 || dst->channels != 1)
	{
		printf("ERROR -> vc_gray_edge_sobel():\n\tNot Gray Image!\n");
		getchar();
		return 0;
	}

	size = width * height;

	for (y = 1; y < height - 1; y++)
	{
		for (x = 1; x < width - 1; x++)
		{
			posA = (y - 1) * bytesperline + (x - 1);
			posB = (y - 1) * bytesperline + x;
			posC = (y - 1) * bytesperline + (x + 1);
			posD = y * bytesperline + (x - 1);
			posX = y * bytesperline + x;
			posE = y * bytesperline + (x + 1);
			posF = (y + 1) * bytesperline + (x - 1);
			posG = (y + 1) * bytesperline + x;
			posH = (y + 1) * bytesperline + (x + 1);


			sumx = datasrc[posA] * -1;
			sumx += datasrc[posD] * -2;
			sumx += datasrc[posF] * -1;

			sumx += datasrc[posC] * +1;
			sumx += datasrc[posE] * +2;
			sumx += datasrc[posH] * +1;
			sumx = sumx / 3; // 3 = 1 + 1 + 1

			sumy = datasrc[posA] * -1;
			sumy += datasrc[posB] * -2;
			sumy += datasrc[posC] * -1;

			sumy += datasrc[posF] * +1;
			sumy += datasrc[posG] * +2;
			sumy += datasrc[posH] * +1;
			sumy = sumy / 3; // 3 = 1 + 1 + 1

			datadst[posX] = (unsigned char)sqrt((double)(sumx * sumx + sumy * sumy));
		}
	}

	// Compute a grey level histogram
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			hist[datadst[y * bytesperline + x * channels]]++;
		}
	}

	// Threshold at the middle of the occupied levels
	histmax = 0;
	for (i = 0; i <= 255; i++)
	{
		histmax += hist[i];

		// th = Prewitt Threshold
		if (histmax >= (((float)size) * th)) break;
	}
	histthreshold = i;


	// Apply the threshold
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			posX = y * bytesperline + x * channels;

			if (datadst[posX] >= histthreshold) datadst[posX] = 255;
			else datadst[posX] = 0;
		}
	}

	return 1;
}

int vc_gray_lowpass_mean_filter(IVC* src, IVC* dst) {

	return 1;
}

//Filtro passo-alto b�sico de imagem gray
int vc_gray_highpass_filter2(IVC* src, IVC* dst) //codigo do prof
{
	unsigned char* datasrc = (unsigned char*)src->data;
	unsigned char* datadst = (unsigned char*)dst->data;
	int width = src->width;
	int height = src->height;
	int bytesperline = src->bytesperline;
	int channels = src->channels;
	int x, y;
	long int posX, posA, posB, posC, posD, posE, posF, posF, posG, posH;
	int sum;

	//Verifica��o de erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL))
		return 0;

	if ((src->width != dst->width) || (src->height != dst->height) || (src->channels != dst->channels))
		return 0;

	if (channels != 1)
		return 0;

	for (y = 1; y < height - 1; y++)
	{
		for (x = 1; x < width - 1; x++)
		{
			posA = (y - 1) * bytesperline + (x - 1) * channels;
			posB = (y - 1) * bytesperline + x * channels;
			posC = (y - 1) * bytesperline + (x + 1) * channels;
			posD = y * bytesperline + (x - 1) * channels;
			posX = y * bytesperline + x * channels;
			posE = y * bytesperline + (x + 1) * channels;
			posF = (y + 1) * bytesperline + (x - 1) * channels;
			posG = (y + 1) * bytesperline + x * channels;
			posH = (y + 1) * bytesperline + (x + 1) * channels;

			sum = datasrc[posA] * -1;
			sum += datasrc[posB] * -1;
			sum += datasrc[posC] * -1;
			sum += datasrc[posD] * -1;
			sum += datasrc[posX] * 10;
			sum += datasrc[posE] * -1;
			sum += datasrc[posF] * -1;
			sum += datasrc[posG] * -1;
			sum += datasrc[posH] * -1;

			datadst[posX] = (unsigned char)((float)abs(sum) / (float)9);
		}
	}
	return 1;

}

int vc_gray_highpass_filter_enhance(IVC* src, IVC* dst)
{
	unsigned char* datasrc = (unsigned char*)src->data;
	unsigned char* datadst = (unsigned char*)dst->data;
	int width = src->width;
	int height = src->height;
	int bytesperline = src->bytesperline;
	int channels = src->channels;
	int x, y;
	long int posX, posA, posB, posC, posD, posE, posF, posF, posG, posH;
	int sum;

	//Verifica��o de erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL))
		return 0;

	if ((src->width != dst->width) || (src->height != dst->height) || (src->channels != dst->channels))
		return 0;

	if (channels != 1)
		return 0;

	for (y = 1; y < height - 1; y++)
	{
		for (x = 1; x < width - 1; x++)
		{
			posA = (y - 1) * bytesperline + (x - 1) * channels;
			posB = (y - 1) * bytesperline + x * channels;
			posC = (y - 1) * bytesperline + (x + 1) * channels;
			posD = y * bytesperline + (x - 1) * channels;
			posX = y * bytesperline + x * channels;
			posE = y * bytesperline + (x + 1) * channels;
			posF = (y + 1) * bytesperline + (x - 1) * channels;
			posG = (y + 1) * bytesperline + x * channels;
			posH = (y + 1) * bytesperline + (x + 1) * channels;

			sum = datasrc[posA] * -1;
			sum += datasrc[posB] * -2;
			sum += datasrc[posC] * -1;
			sum += datasrc[posD] * -2;
			sum += datasrc[posX] * 12;
			sum += datasrc[posE] * -2;
			sum += datasrc[posF] * -1;
			sum += datasrc[posG] * -2;
			sum += datasrc[posH] * -1;

			//datadst[posX] = (unsigned char)((float)abs(sum) / (float)16);
			datadst[posX] = (unsigned char)MIN(MAX((float)datasrc[posX] + ((float)sum / (float)16) * 6.0, 0), 255);
		}
	}

	return 1;
}

//ver versao com o offset do prof e tentativa de resolu��o
int vc_gray_lowpass_gaussian_filter(IVC* src, IVC* dst, int kernel, int sigma) // colcoar float sigma
{

	unsigned char* datasrc = (unsigned char*)src->data;
	unsigned char* datadst = (unsigned char*)dst->data;
	int width = src->width;
	int height = src->height;

	int bytesperline = src->bytesperline;
	int channels = src->channels;

	int x, y, kx, ky;
	int offset = (kernel - 1) / 2;
	int max, min, grayLevels = 256;
	long int pos, posk, total;
	float parte_1 = 0, parte_2 = 0;
	unsigned char threshold;

	//Verifica��o de erros
	if ((src->width <= 0) || (src->height <= 0) || (src->data == NULL))
		return 0;

	if ((src->width != dst->width) || (src->height != dst->height) || (src->channels != dst->channels))
		return 0;

	if (channels != 1)
		return 0;

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperline + x * channels;

			max = 0;
			min = 255;

			// NxM Vizinhos
			for (ky = -offset; ky <= offset; ky++)
			{
				for (kx = -offset; kx <= offset; kx++)
				{
					if ((y + ky >= 0) && (y + ky < height) && (x + kx >= 0) && (x + kx < width))
					{
						posk = (y + ky) * bytesperline + (x + kx) * channels;

						posk = (y + ky) * bytesperline + (x + kx) * channels;
						parte_1 = (1 / (2 * 3, 14159 * sigma * sigma));
						//printf("parte1 %d \n", parte_1);

						parte_2 = exp(-(((kx * kx) + (ky * ky)) / (2 * sigma * sigma)));

						total += parte_1 * parte_2 * datadst[posk];

						//preciso normalizar..

					}
				}
			}
			//printf("total %d \n", total);
			datadst[pos] = total;


		}
	}

	return 1;
}



//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//       FUN��ES: desenvolvidas para o trabalho Pratico 1
// 
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//Desenha em quadrado no blob encontrado
int vc_perimeter_blobs(IVC* src, int bwidth, int bheight, int bx, int by)
{
	unsigned char* data = (unsigned char*)src->data;
	int width = src->width;
	int height = src->height;
	int bytesperline = src->bytesperline;
	int channels = src->channels;
	int x, y, i;
	long int pos;

	//verificar erros
	if (src == NULL)
	{
		printf("ERROR -> vc_perimeter_blobs():\n\tImage is empty!\n");
		getchar();
		return 0;
	}

	if (src->width <= 0 || src->height <= 0 || src->data == NULL)
	{
		printf("ERROR -> vc_perimeter_blobs():\n\tDimensoes or data are missing!\n");
		getchar();
		return 0;
	}


	for (x = 0; x < width; x++)
	{
		for (y = 0; y < height; y++)
		{
			pos = y * bytesperline + x * channels;

			if (x >= bx - 2 && x <= (bx + bwidth) && y == by - 2 || x >= bx - 2 && x <= (bx + bwidth) && y == by + bheight)
			{
				data[pos] = 255;
				data[pos + 1] = 255;
				data[pos + 2] = 255;
			}
			if (x == bx - 2 && y >= by - 2 && y <= (by + bheight) || x == bx + bwidth && y >= by - 2 && y <= (by + bheight))
			{
				data[pos] = 255;
				data[pos + 1] = 0;
				data[pos + 2] = 0;
			}

		}
	}

	return 1;
}

//Marca o centro de massa do blob encontrado
int vc_centro_blobs(IVC* src, int bx, int by)
{
	unsigned char* data = (unsigned char*)src->data;
	int width = src->width;
	int height = src->height;
	int bytesperline = src->bytesperline;
	int channels = src->channels;
	int x, y, i;
	long int pos;

	//verificar erros
	if (src == NULL)
	{
		printf("ERROR -> vc_centro_blobs():\n\tImage is empty!\n");
		getchar();
		return 0;
	}

	if (src->width <= 0 || src->height <= 0 || src->data == NULL)
	{
		printf("ERROR -> vc_centro_blobs():\n\tDimensoes or data are missing!\n");
		getchar();
		return 0;
	}

	for (x = 0; x < width; x++)
	{
		for (y = 0; y < height; y++)
		{
			pos = y * bytesperline + x * channels;

			if (x == bx && y == by)
			{
				data[pos] = 255;
				data[pos + 1] = 0;
				data[pos + 2] = 0;
			}

		}
	}

	return 1;
}

//Pixels com tons de cinza superiores ao Tone ficao pretos
int vc_gray_select(IVC* srcdst, int tone)
{
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

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < with; x++)
		{
			pos = y * bytesperline + x * channels;

			if (data[pos] > tone) {
				data[pos] = 0;
			}
		}
	}

	return 1;
}

//Remove pixels n�o relevantes
int vc_transposal(IVC* src, IVC* dst) {
	unsigned char* datasrc = (unsigned char*)src->data;
	int bytesperlin_src = src->width * src->channels;
	int channels_src = src->channels;
	int width = src->width;
	int height = src->height;
	long int pos;
	int x, y;

	//verificar erros
	if (src == NULL)
	{
		printf("ERROR -> vc_rgb_to_gray():\n\tImage is empty!\n");
		getchar();
		return 0;
	}

	if (dst == NULL)
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

	if (dst->width <= 0 || dst->height <= 0 || dst->data == NULL)
	{
		printf("ERROR -> vc_rgb_to_gray():\n\tDimensoes or data are missing!\n");
		getchar();
		return 0;
	}


	
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			pos = y * bytesperlin_src + x * channels_src;

			if (datasrc[pos] == 0) {
				
				dst->data[pos] = 0;
			}
		}
	}

	return 1;
}