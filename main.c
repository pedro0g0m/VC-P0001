#include <stdio.h>
#include "vc.h"

//Abrir imagem, alterar e gravar em novo ficheiro
int main(void)
{
//Implementar algumas funcões para comecar a por a mao na massa
#pragma region Aula26-02

	//Variaveis 
	/*IVC* image;
	int x, y, i;
	long int pos;*/

#pragma region Invert
//	
//	image = vc_read_image("Images/FLIR/flir-01.pgm");
//	if (image == NULL)
//	{
//		printf("ERROR -> vc_read_image(): \n\tFile not found\n");
//		getchar();
//		return 0;
//	}
//
//	for (i = 0; i < image->bytesperline * image->height; i += image->channels)
//	{
//		image->data[i] = 255 - image->data[i];
//	}
//
//	vc_write_image("vc-0001.pgm", image);
//	vc_image_free(image);
#pragma endregion

#pragma region teste.pbm
	//image = vc_image_new(320, 280, 1, 1);
	//if(image == NULL) {
	//	printf("ERROR -> vc_image_new():\n\t Out of memory!\n");
	//	getchar();
	//	return 0;
	//}

	//for (y = 0; y < image->height; y++)
	//{
	//	for (x = 0; x < image->width; x++)
	//	{
	//		pos = y * image->bytesperline + x * image->channels;
	//		if ((x <= image->width / 2) && (y <= image->height / 2))
	//		{
	//			image->data[pos] = 1;
	//		}
	//		else if ((x > image->width / 2) && (y > image->height / 2))
	//		{
	//			image->data[pos] = 1;
	//		}

	//		else
	//		{
	//			image->data[pos] = 0;
	//		}
	//	}
	//}
	//vc_write_image("teste.pbm", image);
	//vc_image_free(image);
//	
#pragma endregion

#pragma region teste2.pbm
//
//	image = vc_read_image("teste.pbm");
//
//	if (image == NULL)
//	{
//		printf("ERROR -> vc_read_image():\n\tOut of memory!\n");
//		getchar();
//		return 0;
//	}
//
//	for (i = 0; i < image->bytesperline * image->height; i += image->channels)
//	{
//		if (image->data[i] == 1)
//		{
//			image->data[i] = 0;
//		}
//
//		else
//		{
//			image->data[i] = 1;
//		}
//	}
//
//	vc_write_image("teste2.pbm", image);
//	vc_image_free(image);
#pragma endregion

#pragma region CreateImageGradiente1
 
	//image = vc_image_new(256, 256, 1, 255);

	//if (image == NULL) {
	//	printf("ERROR -> vc_image_new():\n\t Out of memory!\n");
	//	getchar();
	//	return 0;
	//}

	//for (y = 0; y < image->height; y++)
	//{
	//	for (x = 0; x < image->width; x++)
	//	{
	//		pos = y * image->bytesperline + x * image->channels;
	//		image->data[pos] = x;
	//	}
	//}

	//vc_write_image("gradiente1.pbm", image);
	//vc_image_free(image);

#pragma endregion

#pragma region CreateImageGradiente2

	//image = vc_image_new(256, 256, 1, 255);

	//if (image == NULL)
	//{
	//	printf("ERRO -> cv_image_new():\n\tOut of memory!\n");
	//	getchar();
	//	return 0;
	//}

	//for (y = 0; y < image->height; y++)
	//{
	//	for (x = 0; x < image->width; x++)
	//	{
	//		pos = y * image->bytesperline + x * image->channels;

	//		image->data[pos] = y;
	//	}
	//}

	//vc_write_image("gradiente2.pbm", image);
	//vc_image_free(image);
#pragma endregion

#pragma region CreateImageGradiente3

	//image = vc_image_new(256, 256, 1, 255);

	//if (image == NULL)
	//{
	//	printf("ERRO -> cv_image_new():\n\tOut of memory!\n");
	//	getchar();
	//	return 0;
	//}

	//for (y = 0; y < image->height; y++)
	//{
	//	for (x = 0; x < image->width; x++)
	//	{
	//		pos = y * image->bytesperline + x * image->channels;

	//		image->data[pos] = (x + y) / 2;
	//	}
	//}

	//vc_write_image("gradiente3.pbm", image);
	//vc_image_free(image);
#pragma endregion

//	printf("Press any key to exit...\n");
//	getchar();

#pragma endregion

//Implementar duas funcoes na biblioteca vc.c (vc_grey_negative) (vc_rgb_negative)
#pragma region Aula03-03

	//Variaveis 
	IVC* image;

#pragma region GraytoNegative

	image = vc_read_image("Images/OldClassic/bridge.pgm");

	if (image == NULL)
	{
		printf("ERROR -> vc_read_image(): \n\tFile not found\n");
		getchar();
		return 0;
	}

	vc_gray_negative(image);
	vc_write_image("graynegative.ppm", image);
	vc_image_free(image);

#pragma endregion

#pragma region RGBtoNegative

	image = vc_read_image("Images/Classic/airplane.ppm");

	if (image == NULL)
	{
		printf("ERROR -> vc_read_image(): \n\tFile not found\n");
		getchar();
		return 0;
	}

	vc_rgb_negative(image);
	vc_write_image("rgbnegative.ppm", image);
	vc_image_free(image);

#pragma endregion

	printf("Press any key to exit...\n");
	getchar();

#pragma endregion

	return 0;
	
}



	
