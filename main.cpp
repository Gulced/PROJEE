#include <stdio.h>
#include <stdlib.h>
#include <string.h>
3
#define MAX_LINE_LENGTH 100
#define KOTA_LIMIT_GB 6      // Kota 6 GB (6000 MB) olarak belirlenmiş
#define MB_TO_GB 1024        // 1 GB = 1024 MB
#define KOTA_ASIM_UCRETI_PER_MB 0.011719  // 1 MB başına ücret

const char *months[] = {"Ocak", "Şubat", "Mart", "Nisan", "Mayıs", "Haziran",
                        "Temmuz", "Ağustos", "Eylül", "Ekim", "Kasım", "Aralık"};

int getMonthIndex(const char *userMonth);
void readDataFromFile(const char *filePath, int monthIndex, double *totalUpload, double *totalDownload);
double calculateOverageFee(double totalDataMB);
void writeInvoiceToFile(const char *filePath, int monthIndex, double totalUpload, double totalDownload, double overageFee);

int main() {
    char userMonth[20];
    int monthIndex = -1;
    double totalUpload = 0.0, totalDownload = 0.0;
    double overageFee = 0.0;

    while (1) {
        printf("Fatura oluşturmak için ay ismini girin (örn. Ocak, Şubat, ...): ");
        fgets(userMonth, sizeof(userMonth), stdin);
        userMonth[strcspn(userMonth, "\n")] = '\0';

        monthIndex = getMonthIndex(userMonth);
        if (monthIndex == -1) {
            printf("Geçersiz bir ay ismi girdiniz! Başka bir ay girmeyi deneyin.\n");
            continue;
        }

        readDataFromFile("/Users/gulcekoc/CLionProjects/PROJEE/adsl.txt", monthIndex, &totalUpload, &totalDownload);

        double totalDataMB = (totalUpload + totalDownload);
        overageFee = calculateOverageFee(totalDataMB);

        writeInvoiceToFile("/Users/gulcekoc/CLionProjects/PROJEE/fatura.txt", monthIndex, totalUpload, totalDownload, overageFee);

        char tekrar;
        printf("\nBaşka bir ay girmek ister misiniz? (Evet: e, Hayır: h): ");
        scanf(" %c", &tekrar);
        if (tekrar == 'h' || tekrar == 'H') {
            printf("Çıkılıyor...\n");
            break;
        }
        getchar();
    }

    return 0;
}

int getMonthIndex(const char *userMonth) {
    for (int i = 0; i < 12; i++) {
        if (strcasecmp(userMonth, months[i]) == 0) {
            return i;
        }
    }
    return -1;
}

void readDataFromFile(const char *filePath, int monthIndex, double *totalUpload, double *totalDownload) {
    FILE *adslFile = fopen(filePath, "r");
    if (adslFile == NULL) {
        perror("adsl.txt dosyası açılamadı");
        exit(1);
    }

    char line[MAX_LINE_LENGTH];
    double upload, download;

    while (fgets(line, sizeof(line), adslFile) != NULL) {
        char monthName[20];
        int day;

        if (sscanf(line, "%s %d %lf %lf", monthName, &day, &upload, &download) == 4) {
            if (strcasecmp(monthName, months[monthIndex]) == 0) {
                *totalUpload += upload;
                *totalDownload += download;
            }
        }
    }
    fclose(adslFile);
}

double calculateOverageFee(double totalDataMB) {
    if (totalDataMB > KOTA_LIMIT_GB * MB_TO_GB) {
        return (totalDataMB - KOTA_LIMIT_GB * MB_TO_GB) * KOTA_ASIM_UCRETI_PER_MB;
    }
    return 0.0;
}

void writeInvoiceToFile(const char *filePath, int monthIndex, double totalUpload, double totalDownload, double overageFee) {
    FILE *faturaFile = fopen(filePath, "w");
    if (faturaFile == NULL) {
        perror("fatura.txt dosyası oluşturulamadı");
        exit(1);
    }

    fprintf(faturaFile, "%s 2012\n", months[monthIndex]);
    fprintf(faturaFile, "Toplam yükleme (upload) : %.2f GB\n", totalUpload / MB_TO_GB);
    fprintf(faturaFile, "Toplam indirme (download) : %.2f GB\n", totalDownload / MB_TO_GB);
    fprintf(faturaFile, "Kota aşım ücreti : %.2f TL\n", overageFee);

    printf("%s 2012 Faturası:\n", months[monthIndex]);
    printf("Toplam yükleme (upload) : %.2f GB\n", totalUpload / MB_TO_GB);
    printf("Toplam indirme (download) : %.2f GB\n", totalDownload / MB_TO_GB);
    printf("Kota aşım ücreti : %.2f TL\n", overageFee);

    fclose(faturaFile);
}
