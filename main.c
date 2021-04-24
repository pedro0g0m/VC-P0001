#include <stdio.h>
#include "vc.h"
#include "labelling.h"
#include <stdlib.h>
#include <malloc.h>


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

#pragma endregion

//Implementar duas funcoes na biblioteca vc.c (vc_grey_negative) (vc_rgb_negative)
#pragma region Aula03-03

//Variaveis 
//IVC* image;

#pragma region GraytoNegative

/*image = vc_read_image("Images/OldClassic/bridge.pgm");

if (image == NULL)
{
	printf("ERROR -> vc_read_image(): \n\tFile not found\n");
	getchar();
	return 0;
}

vc_gray_negative(image);
vc_write_image("graynegative.ppm", image);
vc_image_free(image);*/

#pragma endregion

#pragma region RGBtoNegative

/*image = vc_read_image("Images/Classic/airplane.ppm");

if (image == NULL)
{
	printf("ERROR -> vc_read_image(): \n\tFile not found\n");
	getchar();
	return 0;
}

vc_rgb_negative(image);
vc_write_image("rgbnegative.ppm", image);
vc_image_free(image);*/

#pragma endregion

#pragma endregion

//Implementacao de funcoes para isolar um cor (R||G||B) e outra para RGB to GRAY
#pragma region Aula05-03

//Variaveis 
/*IVC* image, *src, *dst;*/

#pragma region Red OUT

/*image = vc_read_image("Images/Classic/pepper.ppm");

if (image == NULL)
{
	printf("ERROR -> vc_read_image(): \n\tFile not found\n");
	getchar();
	return 0;
}

vc_rgb_get_red_gray(image);
vc_write_image("redout.ppm", image);
vc_image_free(image);*/

#pragma endregion

#pragma region Green OUT

/*image = vc_read_image("Images/Classic/pepper.ppm");

if (image == NULL)
{
	printf("ERROR -> vc_read_image(): \n\tFile not found\n");
	getchar();
	return 0;
}

vc_rgb_get_green_gray(image);
vc_write_image("greenout.ppm", image);
vc_image_free(image)*/;

#pragma endregion

#pragma region Blue OUT

/*image = vc_read_image("Images/Classic/pepper.ppm");

if (image == NULL)
{
	printf("ERROR -> vc_read_image(): \n\tFile not found\n");
	getchar();
	return 0;
}

vc_rgb_get_blue_gray(image);
vc_write_image("blueout.ppm", image);
vc_image_free(image);*/


#pragma endregion

#pragma region RGB to GRAY

//src = vc_read_image("Images/Classic/pepper.ppm");

////dimensoes img src == dst
//int width = src->width;
//int height = src->height;

//dst = vc_image_new(width, height, 1, 255);

//if (src == NULL)
//{
//	printf("ERROR -> vc_read_image(): \n\tFile not found\n");
//	getchar();
//	return 0;
//}

//vc_rgb_to_gray(src, dst);
//vc_write_image("RGBtoGRAY.ppm", dst);
//vc_image_free(src);
//vc_image_free(dst);

#pragma endregion

#pragma endregion

//Explicacao de HSV implementatacao de RGBtoHSV ficou por terminar a HSV_Segmentation 
#pragma region Aula10-03

//Variaveis
//IVC* image;

#pragma region RGBtoHSV
/*image = vc_read_image("Images/Imagens de Teste para HSV/HSVTestImage01.ppm");

if (image == NULL)
{
	printf("ERROR -> vc_read_image(): \n\tFile not found\n");
	getchar();
	return 0;
}

vc_rgb_to_hsv(image);
vc_write_image("RGBtoHSV.ppm", image);
vc_image_free(image);*/
#pragma endregion

#pragma region hsv_segmentation

//COMPLETAR NA PROXIMA AULA 
#pragma endregion

#pragma endregion

//Continuação da funcao hsv_segmentation e implementacao da funcao gray2rgb
#pragma region Aula12-03

//Variaveis
//IVC* image, *src, *dst, *new;

#pragma region hsv_segmentation
//image = vc_read_image("Images/Imagens de Teste para HSV/HSVTestImage01.ppm");
//if (image == NULL)
//{
//	printf("ERROR -> vc_read_image():\n\tFile not found!\n");
//	getchar();
//	return 0;
//}

//vc_rgb_to_hsv(image);
////vc_hsv_segmentation(image, 30, 70, 50, 100, 60, 100); //Amarelo

//vc_hsv_segmentation(image, 0, 360, 0, 20, 85, 100);

//vc_write_image("vc-hsv2rgb.ppm", image);

//vc_image_free(image);

//system("cmd /c start FilterGear Images/Imagens de Teste para HSV/HSVTestImage01.ppm");
//system("FilterGear vc-hsv2rgb.ppm");


#pragma endregion

#pragma region gray_to_RGB

//src = vc_read_image("Images/gray2rgb_res/gray2rgb_testpat.ppm");
//
//if (src == NULL)
//{
//	printf("ERROR -> vc_read_image(): \n\tFile not found\n");
//	getchar();
//	return 0;
//}

////dimensoes img src == dst
//int width = src->width;
//int height = src->height;


//dst = vc_image_new(width, height, 1, 255);

//vc_rgb_to_gray(src, dst);

//new = vc_image_new(width, height, 3, 255);

//vc_scale_gray_to_rgb(dst, new);
//vc_write_image("GRAYtoRGB.ppm", new);

//vc_image_free(src);
//vc_image_free(dst);
//vc_image_free(new);

////system("cmd /c start FilterGear Images/gray2rgb_res/gray2rgb_testpat.ppm");
////system("FilterGear GRAYtoRGB.ppm");

#pragma endregion


#pragma endregion

//Tranformação de Pic cinza em binary usando varios metodos
#pragma region Aula17-03

//Variaveis
//IVC* image, * src, * dst;

#pragma region vc_gray_to_binary
/*image = vc_read_image("Images/Imagens de Teste para Segmentação/thresholdingImg.pgm");

if (image == NULL)
{
	printf("ERROR -> vc_read_image(): \n\tFile not found\n");
	getchar();
	return 0;
}

vc_gray_to_binary(image, 100);
vc_write_image("binary+thresh.ppm", image);
vc_image_free(image);

system("FilterGear binary+thresh.ppm");*/
#pragma endregion

#pragma region vc_gray_to_binary_global_mean
/*image = vc_read_image("Images/Imagens de Teste para Segmentação/thresholdingImg.pgm");

if (image == NULL)
{
	printf("ERROR -> vc_read_image(): \n\tFile not found\n");
	getchar();
	return 0;
}

vc_gray_to_binary_global_mean(image);
vc_write_image("threshold.ppm", image);
vc_image_free(image);

system("FilterGear threshold.ppm");*/
#pragma endregion

#pragma region vc_gray_to_binary_midpoint
//src = vc_read_image("Images/Imagens de Teste para Segmentação/thresholdingImg.pgm");

////dimensoes img src == dst
//int width = src->width;
//int height = src->height;

//dst = vc_image_new(width, height, 1, 255);

//if (src == NULL)
//{
//	printf("ERROR -> vc_read_image(): \n\tFile not found\n");
//	getchar();
//	return 0;
//}

//vc_gray_to_binary_midpoint(src, dst, 35);
//vc_write_image("binarymidpoint.ppm", dst);
//vc_image_free(src);
//vc_image_free(dst);

//system("FilterGear binarymidpoint.ppm");
#pragma endregion

#pragma endregion

// Continuacao da implementacao das funcoes midpoint, niblack
#pragma region Aula19-03

//Variaveis
//IVC* image, * src, * dst;

#pragma region vc_gray_to_binary_niblack
//src = vc_read_image("Images/Imagens de Teste para Segmentação/thresholdingImg.pgm");

////dimensoes img src == dst
//int width = src->width;
//int height = src->height;

//dst = vc_image_new(width, height, 1, 255);

//if (src == NULL)
//{
//	printf("ERROR -> vc_read_image(): \n\tFile not found\n");
//	getchar();
//	return 0;
//}

//vc_gray_to_binary_niblack(src, dst, 51, -0.2);
//vc_write_image("binaryniblack.ppm", dst);
//vc_image_free(src);
//vc_image_free(dst);

#pragma endregion

#pragma endregion

// Continuação das atividade da aula anterior
#pragma region Aula23-03
//Continuação da implementação da funcao niblack
#pragma endregion

//Implementação das funçoes de erosao e dilatação
#pragma region Aula26-03

#pragma endregion

//Blobs ou etiquetas
#pragma region Aula07-04
	//Funcao ja implementada mas os testes serao feitos na proxima aula
#pragma endregion

//apresentaçao do trabalho pratico + continuaçao da aula anterior
#pragma region Aula09-04

	//Variaveis
	//IVC* image, *src, *dst;

#pragma region vc_binary_blob_labelling

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//              Thresh global mean + negative

//image = vc_read_image("Images/Imagens de Teste para Segmentação/coins.pgm");

//if (image == NULL)
//{
//	printf("ERROR -> vc_read_image(): \n\tFile not found\n");
//	getchar();
//	return 0;
//}

//vc_gray_to_binary_global_mean(image);
//vc_gray_negative(image);
////++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
////				Close(k = 3) + Open(k = 3)
//
////dimensoes img src == dst
//int width = image->width;
//int height = image->height;

//src = vc_image_new(width, height, 1, 255);

//vc_binary_close(image, src, 3);

////dimensoes img src == dst
//int width1 = src->width;
//int height1 = src->height;

//dst = vc_image_new(width1, height1, 1, 255);

//vc_binary_open(src, dst, 3);

//vc_image_free(image);
//vc_image_free(src);

////++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
////				Etiquetagem (Labelling) 
//
////dimensoes img src == dst
//int width2 = dst->width;
//int height2 = dst->height;

//image = vc_image_new(width2, height2, 1, 255);
//src = vc_image_new(width2, height2, 1, 255);

//vc_binary_blob_labelling(dst, image, src);

////++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
////
//vc_write_image("teste.ppm", image);
//vc_image_free(dst);
//vc_image_free(image);
//

//system("FilterGear teste.ppm");


#pragma endregion

#pragma endregion

//blob_info 
#pragma region Aula14-04

#pragma region vc_binary_blob_info

////++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
////              Thresh global mean + negative

//IVC* image = vc_read_image("Images/Imagens de Teste para Segmentação/coins.pgm");

//if (image == NULL)
//{
//	printf("ERROR -> vc_read_image(): \n\tFile not found\n");
//	getchar();
//	return 0;
//}

//vc_gray_to_binary_global_mean(image);
//vc_gray_negative(image);
////++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
////				Close(k = 3) + Open(k = 3)


//IVC* src = vc_image_new(image->width, image->height, 1, 255);

//vc_binary_close(image, src, 3);


//IVC* dst = vc_image_new(src->width, src->height, 1, 255);

//vc_binary_open(src, dst, 3);

//vc_image_free(image);
//vc_image_free(src);

//vc_write_image("teste.ppm", dst);

////++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
////				Etiquetagem (Labelling) 

//int nlabels = 0;
//IVC* labelling = vc_image_new(dst->width, dst->height, 1, 255);
//OVC* blobs = vc_binary_blob_labelling(dst, labelling, &nlabels);

////++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

////++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
////				Etiquetagem (BLOBS) 

//int i;

//vc_binary_blob_info(labelling, blobs, nlabels);

//printf("\n \n Dados das Moedas \n \n");
//printf("Numero de moedas: %i \n", nlabels);

//for (i = 0; i < nlabels; i++) {
//	

//	printf("Perimetro: %d \n", blobs[i].perimeter);
//	printf("Area: %d \n", blobs[i].area);
//	printf("Centro de massa: \n X = %d \t Y = %d \n \n", blobs[i].xc, blobs[i].yc);

//	vc_perimeter_blobs(labelling, blobs[i].width, blobs[i].height, blobs[i].x, blobs[i].y);

//}

////++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//vc_write_image("testelabelling.ppm", labelling);
//vc_image_free(dst);
//vc_image_free(labelling);


//system("FilterGear testelabelling.ppm");


#pragma endregion

#pragma endregion

//histograma 
#pragma region Aula16-04 

//Variaveis
/*IVC* image, *src, *dst;*/

#pragma region vc_gray_histogram

//image = vc_read_image("Images/Imagens de Teste para Histograma/tire.pgm");

//if (image == NULL)
//{
//	printf("ERROR -> vc_read_image(): \n\tFile not found\n");
//	getchar();
//	return 0;
//}

////dimensoes img src == dst
//int width = image->width;
//int height = image->height;

//dst = vc_image_new(width, height, 1, 255);
//src = vc_image_new(width, height, 1, 255);

////Histograma da foto original
//vc_gray_histogram(image, dst);
//vc_write_image("histogram.pgm", dst);

////equalization
//vc_gray_histogram_equalization(image, src);
//vc_write_image("histogramequalization.pgm", src);

//vc_image_free(dst);

//dst = vc_image_new(width, height, 1, 255);

////Histograma da foto equalizada
//vc_gray_histogram(src, dst);
//vc_write_image("histogram1.pgm", dst);

//vc_image_free(image);
//vc_image_free(src);
//vc_image_free(dst);


#pragma endregion

#pragma endregion

// prewitt + sobel 
#pragma region Aula21-04

//Variaveis
//IVC* image, *dst;

//Perguntar ao prof pk tudo branco!!!
#pragma region vc_gray_edge_prewitt

	/*image = vc_read_image("Images/OldClassic/cameraman.pgm");

	if (image == NULL)
	{
		printf("ERROR -> vc_read_image(): \n\tFile not found\n");
		getchar();
		return 0;
	}

	dst = vc_image_new(image->width, image->height, 1, 255);

	vc_gray_edge_prewitt(image, dst, 0.80);
	vc_write_image("prewitt.pgm", dst);

	system("FilterGear prewitt.pgm");

	vc_image_free(image);
	vc_image_free(dst);*/

#pragma endregion

#pragma region vc_gray_edge_sobel
	/*image = vc_read_image("Images/OldClassic/cameraman.pgm");

	if (image == NULL)
	{
		printf("ERROR -> vc_read_image(): \n\tFile not found\n");
		getchar();
		return 0;
	}

	dst = vc_image_new(image->width, image->height, 1, 255);

	vc_gray_edge_sobel(image, dst, 0.85);

	vc_write_image("sobel.pgm", dst);
	system("FilterGear sobel.pgm");

	vc_image_free(image);
	vc_image_free(dst);*/

#pragma endregion

#pragma region RGB -> Gray + prewitt

	//image = vc_read_image("Images/Classic/lenna.ppm");

	//if (image == NULL)
	//{
	//	printf("error -> vc_read_image():\n\tFile not found!\n");
	//	getchar();
	//	return 0;
	//}

	////RGB -> GRAY
	//IVC* gray = vc_image_new(image->width, image->height, 1, 255);
	//vc_rgb_to_gray(image, gray);

	////PREWITT
	//IVC* edge = vc_image_new(gray->width, gray->height, 1, 255);
	//vc_gray_edge_prewitt(gray, edge, 0.90);

	//vc_write_image("edge.pgm", edge);
	//system("FilterGear edge.pgm");

	//vc_image_free(image);
	//vc_image_free(gray);
	//vc_image_free(edge);
#pragma endregion 


#pragma endregion

	#pragma region Tp1.2

	IVC* srcimage, * wrkimage, * cloimage, * eximage, * openimage, * grayimage, * P2image;

	srcimage = vc_read_image("Images/img2.ppm");
	wrkimage = vc_read_image("Images/img2.ppm");
	P2image = vc_read_image("Images/img2.ppm");

	if (srcimage == NULL)
	{
		printf("ERROR -> vc_read_image(): \n\tFile not found\n");
		getchar();
		return 0;
	}

	//P2image = vc_image_new(srcimage->width, srcimage->height, 1, 255);
	grayimage = vc_image_new(srcimage->width, srcimage->height, 1, 255);
	openimage = vc_image_new(srcimage->width, srcimage->height, 1, 1);
	cloimage = vc_image_new(srcimage->width, srcimage->height, 1, 1);
	eximage = vc_image_new(srcimage->width, srcimage->height, 1, 255);
	//labelling = vc_image_new(srcimage->width, srcimage->height, 1, 255);

	// saves image
	//vc_write_image("input.ppm", image);
	//system("cmd /c start FilterGear input.ppm");

	vc_rgb_to_hsv(wrkimage);

	vc_hsv_segmentation(wrkimage, 189, 240, 50, 100, 30, 100);

	vc_rgb_to_gray(wrkimage, grayimage);

	vc_gray_to_binary(grayimage, 50);

	//vc_write_image("P2imageGray.ppm", grayimage);
	//system("FilterGear P2imageGray.ppm");


	vc_binary_close(grayimage, cloimage, 7);

	vc_binary_open(cloimage, openimage, 7);


	//vc_write_image("P2image.ppm", cloimage);
	//system("FilterGear P2image.ppm.ppm");
	//vc_image_free(image);
	vc_image_free(wrkimage);
	vc_image_free(grayimage);
	//vc_image_free(openimage);
	//Dilate


	//save image
	vc_write_image("outputP2.pgm", openimage);
	system("cmd /c start FilterGear outputP2.pgm");


	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//				Etiquetagem (Labelling) 

	int nlabels = 0;
	IVC* labelling = vc_image_new(openimage->width, openimage->height, 1, 255);
	OVC* blobs = vc_binary_blob_labelling(openimage, labelling, &nlabels);

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	//				Etiquetagem (BLOBS) 

	int i, obj = 0;

	vc_binary_blob_info(labelling, blobs, nlabels);

	printf("\n \n Dados do nucleo das celulas \n \n");
	printf("Numero de celulas: %i \n", nlabels);

	for (i = 0; i < nlabels; i++) {

		obj = obj + 1;

		printf("Perimetro: %d \n", blobs[i].perimeter);
		printf("Area: %d \n", blobs[i].area);
		printf("Centro de massa: \n X = %d \t Y = %d \n \n", blobs[i].xc, blobs[i].yc);

		vc_perimeter_blobs(P2image, blobs[i].width, blobs[i].height, blobs[i].x, blobs[i].y);
		vc_centro_blobs(P2image, blobs[i].xc, blobs[i].yc);

	}

	//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	vc_write_image("P2.pgm", P2image);
	system("FilterGear P2.pgm");
	//vc_image_free(open);
	vc_image_free(labelling);
	vc_image_free(openimage);



#pragma endregion



printf("Press any key to exit...\n");
getchar();
return 0;

}