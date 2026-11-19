#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

#include <locale.h>
#ifdef _WIN32
	#include <windows.h>
#endif

// SUBALGORITMOS

void configurarIdioma(void) {
    // 1. Configura a localidade padrão da linguagem C
    if (setlocale(LC_ALL, "pt_BR.UTF-8") == NULL) {
        if (setlocale(LC_ALL, "Portuguese_Brazil.1252") == NULL) {
            setlocale(LC_ALL, "Portuguese");
        }
    }

    // 2. Força o terminal do Windows a usar UTF-8 na entrada e saída
    #ifdef _WIN32
    	SetConsoleCP(65001);       	// Entrada (scanf / fgets)
    	SetConsoleOutputCP(65001); 	// Saída (printf)
    #endif
}

double obter_momento_inercia() {
    int opcao;
    double b, h, d;

    printf("Selecione o formato da seção transversal:\n");
    printf("1 - Retangular\n");
    printf("2 - Quadrada\n");
    printf("3 - Circular\n");
    printf("Opção: ");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1:
            printf("\nDigite a base b (em metros [m]): ");
            scanf("%lf", &b);
            printf("Digite a altura h (em metros [m]): ");
            scanf("%lf", &h);
            return (b < h) ? (h * pow(b, 3)) / 12.0 : (b * pow(h, 3)) / 12.0;

        case 2:
            printf("\nDigite o lado b (em metros [m]): ");
            scanf("%lf", &b);
            return pow(b, 4) / 12.0;

        case 3:
            printf("\nDigite o diametro d (em metros [m]): ");
            scanf("%lf", &d);
            return (M_PI * pow(d, 4)) / 64.0;

        default:
            printf("\nOpção de seção inválida!\n");
            return -1.0;
    }
}

double obter_fator_vinculacao() {
    int opcao;

    printf("\nSelecione o tipo de vinculação:\n");
    printf("1 - Bi-engastada (K = 0.5)\n");
    printf("2 - Engastada e articulada (K = 0.7)\n");
    printf("3 - Bi-articulada (K = 1.0)\n");
    printf("4 - Engastada e livre (K = 2.0)\n");
    printf("Opção: ");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1: return 0.5;
        case 2: return 0.7;
        case 3: return 1.0;
        case 4: return 2.0;
        default:
            printf("\nOpção de vinculação inválida!\n");
            return -1.0;
    }
}

double calcular_carga_critica(double E, double I, double K, double L) {
    return (pow(M_PI, 2) * E * I) / pow((K * L), 2);
}

void processar_deflexao_maxima(double E, double I, double L, double P_critico) {
    int tem_excentricidade;
    double e, P_aplicada, arg_sec, v_max;

    printf("Existe excentricidade (e) na aplicação da carga?\n");
    printf("1 - Sim\n");
    printf("2 - Não\n");
    printf("Opção: ");
    scanf("%d", &tem_excentricidade);

    if (tem_excentricidade == 1) {
        printf("\nDigite o valor da excentricidade e (em metros [m]): ");
        scanf("%lf", &e);
        printf("Digite o valor da carga aplicada P (em Newtons [N]): ");
        scanf("%lf", &P_aplicada);

        if (P_aplicada >= P_critico) {
            printf("\nALERTA: A carga aplicada (%.2lf N) é maior ou igual a carga crítica (%.2lf N).\n", P_aplicada, P_critico);
            printf("A barra sofrera flambagem.\n");
        } else {
            arg_sec = sqrt(P_aplicada / (E * I)) * (L / 2.0);
            v_max = e * ((1.0 / cos(arg_sec)) - 1.0);

            printf("\n--------------------------------------------------\n");
            printf("Deflexão Horizontal Máxima (Vmáx) = %.6e m (%.4lf mm)\n", v_max, v_max * 1000.0);
            printf("--------------------------------------------------\n");
        }
    } else if (tem_excentricidade == 2) {
        printf("\nCálculo finalizado apenas com a Carga Crítica.\n");
    } else {
        printf("\nOpção inválida.\n");
    }
}

// PRINCIPAL

int main() {
    double I, E, L, K, P_critico;
    
    configurarIdioma();

    printf("========================================================\n");
    printf("       CÁLCULO DE FLAMBAGEM E DEFLEXÃO MÁXIMA (SI)\n");
    printf("   *Insira todas as medidas no Sistema Internacional*\n");
    printf("========================================================\n\n");

    I = obter_momento_inercia();
    if (I <= 0) {
        system("pause");
        return 1;
    }

    printf("\nDigite o Modulo de Elasticidade E (em Pascal [Pa], ex: 200e9 para 200 GPa): ");
    scanf("%lf", &E);
    printf("Digite o comprimento da barra L (em metros [m]): ");
    scanf("%lf", &L);

    K = obter_fator_vinculacao();
    if (K <= 0) {
        system("pause");
        return 1;
    }

    P_critico = calcular_carga_critica(E, I, K, L);

    printf("\n------------------------------------------------------------------------\n");
	printf("Momento de Inércia (I) = %.6lf m^4\n", I);
    printf("Carga Crítica (P_critico)  = %.4lf N (%.4lf kN)\n", P_critico, P_critico / 1000.0);
    printf("------------------------------------------------------------------------\n\n");

    processar_deflexao_maxima(E, I, L, P_critico);

    printf("\n");
    system("pause");
    return 0;
}
