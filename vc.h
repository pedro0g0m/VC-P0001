//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//           INSTITUTO POLIT�CNICO DO C�VADO E DO AVE
//                          2011/2012
//             ENGENHARIA DE SISTEMAS INFORM�TICOS
//                    VIS�O POR COMPUTADOR
//
//             [  DUARTE DUQUE - dduque@ipca.pt  ]
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


#define VC_DEBUG


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                   ESTRUTURA DE UMA IMAGEM
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


typedef struct {
	unsigned char *data;
	int width, height;
	int channels;			// Bin�rio/Cinzentos=1; RGB=3
	int levels;				// Bin�rio=1; Cinzentos [1,255]; RGB [1,255]
	int bytesperline;		// width * channels
} IVC;


//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//                    PROT�TIPOS DE FUN��ES
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// FUN��ES: ALOCAR E LIBERTAR UMA IMAGEM
IVC *vc_image_new(int width, int height, int channels, int levels);
IVC *vc_image_free(IVC *image);

// FUN��ES: LEITURA E ESCRITA DE IMAGENS (PBM, PGM E PPM)
IVC *vc_read_image(char *filename);
int vc_write_image(char *filename, IVC *image);

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//            FUN��ES: desenvolvidas nas AULAS
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//Calcula o negativo de uma imagem GREY
int vc_gray_negative(IVC* srcdst);

//Calcula o negativo de uma imagem RGB
int vc_rgb_negative(IVC* srcdst);

//Removes the color RED from a RGB picture
int vc_rgb_get_red_gray(IVC* srcdst);

//Removes the color GREEN from a RGB picture
int vc_rgb_get_green_gray(IVC* srcdst);

//Removes the color BLUE from a RGB picture
int vc_rgb_get_blue_gray(IVC* srcdst);

//Transforme a RGB pi to GRAY
int vc_rgb_to_gray(IVC* src, IVC* dst);

//Converts color RGB to HSV
int vc_rgb_to_hsv(IVC* srcdst);

//Se encontrar a cor fica branco tudo o resto fica preto.ver colorpicker no google para escolher os intervalos
int vc_hsv_segmentation(IVC* srcdst, int hmin, int hmax, int smin, int smax, int vmin, int vmax);

//De uma imagem em cinza para uma imagem RGB
int vc_scale_gray_to_rgb(IVC* src, IVC* dst);

//FUN��O DE BINARIZA��O com threshold manual
int vc_gray_to_binary(IVC* srcdst, int threshold);

//FUN��O DE BINARIZA��O com threshold calculado
int vc_gray_to_binary_global_mean(IVC* srcdst);

//Use prime numbers to kernel
int vc_gray_to_binary_midpoint(IVC* src, IVC* dst, int kernel);

//MUITO LENTO NAO FUNCIONA
int vc_gray_to_binary_niblack(IVC* src, IVC* dst, int kernel, float alfa);

//FUN��O EROS�O
int vc_binary_erode(IVC* src, IVC* dst, int kernel);

// FUN��O DILATA��O
int vc_binary_dilate(IVC* src, IVC* dst, int kernel);

//Funcao de fecho (1-> Dilata 2-> erode)
int vc_binary_close(IVC* src, IVC* dst, int kernel);

//Funcao abrir (1-> erode 2-> dilata)
int vc_binary_open(IVC* src, IVC* dst, int kernel);

//FUN��O C�LCULO DO HISTOGRAMA
int vc_gray_histogram(IVC* src, IVC* srcD);

//FUN��O C�LCULO DO HISTOGRAMA
int vc_gray_histogram_equalization(IVC* src, IVC* srcD);

//Dete��o de contornos Prewitt
int vc_gray_edge_prewitt(IVC* src, IVC* dst, float th);

//Dete��o de contornos Sobel
int vc_gray_edge_sobel(IVC* src, IVC* dst, float th);

//Desenha em quadrado no blob encontrado
int vc_perimeter_blobs(IVC* src, int bwidth, int bheight, int bx, int by);

//Marca o centro de massa do blob encontrado
int vc_centro_blobs(IVC* src, int bx, int by);

//Pixels com tons de cinza superiores ao Tone ficao pretos
int vc_gray_select(IVC* srcdst, int tone);

//Remove partes n�o relevantes da imagem 
int vc_transposal(IVC* src, IVC* dst);

int vc_gray_lowpass_mean_filter(IVC* src, IVC* dst);

int vc_gray_lowpass_gaussian_filter(IVC* src, IVC* dst, int kernel, int sigma);

int vc_gray_highpass_filter_enhance(IVC* src, IVC* dst);

int vc_gray_highpass_filter2(IVC* src, IVC* dst);
