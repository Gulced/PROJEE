#include <stdio.h>
#include <stdlib.h>
#include <string.h>
33^3333
#define MAX_LINE_LENGTH 100

// Kota aşım ücretini ve kota sınırlarını tanımlıyoruz
#define KOTA_LIMIT_GB 6      // Kota 6 GB (6000 MB) olarak belirlenmiş
#define MB_TO_GB 1024        // 1 GB = 1024 MB
#define KOTA_ASIM_UCRETI_PER_MB 0.011719  // 1 MB başına ücret

// Ay isimleri için yardımcı dizi
const char *months[] = {"Ocak", "Şubat", "Mart", "Nisan", "Mayıs", "Haziran",
                        "Temmuz", "Ağustos", "Eylül", "Ekim", "Kasım", "Aralık"};

int main() {
    FILE *adslFile, *faturaFile;
    char line[MAX_LINE_LENGTH];
    double upload, download, totalUpload = 0.0, totalDownload = 0.0;
    double kotaAsimUcreti = 0.0;
    char userMonth[20];
    int monthIndex = -1;

    // Sonsuz döngü ile ay sorulacak
    while (1) {
        // Kullanıcıdan ay adı alıyoruz
        printf("Fatura oluşturmak için ay ismini girin (örn. Ocak, Şubat, ...): ");
        fgets(userMonth, sizeof(userMonth), stdin);
        userMonth[strcspn(userMonth, "\n")] = '\0';  // Yeni satır karakterini temizle

        // Kullanıcının girdiği ayın dizideki indeksini buluyoruz
        for (int i = 0; i < 12; i++) {
            if (strcasecmp(userMonth, months[i]) == 0) {
                monthIndex = i;
                break;
            }
        }

        if (monthIndex == -1) {
            printf("Geçersiz bir ay ismi girdiniz! Başka bir ay girmeyi deneyin.\n");
            continue;  // Hatalı giriş yapılırsa döngü başa döner
        }

        // adsl.txt dosyasının tam yolu
        adslFile = fopen("/Users/gulcekoc/CLionProjects/PROJEE/adsl.txt", "r");
        if (adslFile == NULL) {
            perror("adsl.txt dosyası açılamadı");
            return 1;
        }

        // Dosyayı satır satır okuma
        while (fgets(line, sizeof(line), adslFile) != NULL) {
            char monthName[20];
            int day;

            // Satırdaki verileri alıyoruz
            if (sscanf(line, "%s %d %lf %lf", monthName, &day, &upload, &download) == 4) {
                // Ayın doğru olup olmadığını kontrol ediyoruz
                if (strcasecmp(monthName, months[monthIndex]) == 0) {
                    totalUpload += upload;
                    totalDownload += download;
                }
            }
        }
        fclose(adslFile);  // Dosyayı kapatıyoruz

        // Toplam yükleme ve indirme verilerini hesapla ve kota aşımını kontrol et
        double totalDataMB = (totalUpload + totalDownload);  // MB cinsinden veri miktarı

        // Kota aşımı kontrolü (Kota 6000 MB, eğer aşarsa ücret hesaplanır)
        if (totalDataMB > KOTA_LIMIT_GB * MB_TO_GB) {
            kotaAsimUcreti = (totalDataMB - KOTA_LIMIT_GB * MB_TO_GB) * KOTA_ASIM_UCRETI_PER_MB;
        } else {
            kotaAsimUcreti = 0.0;  // Kota aşımı yoksa 0 TL yazılacak
        }

        // fatura.txt dosyasının tam yolu
        faturaFile = fopen("/Users/gulcekoc/CLionProjects/PROJEE/fatura.txt", "w");
        if (faturaFile == NULL) {
            perror("fatura.txt dosyası oluşturulamadı");
            return 1;
        }

        // Faturayı yazıyoruz
        fprintf(faturaFile, "%s 2012\n", months[monthIndex]);
        fprintf(faturaFile, "Toplam yükleme (upload) : %.2f GB\n", totalUpload / MB_TO_GB);
        fprintf(faturaFile, "Toplam indirme (download) : %.2f GB\n", totalDownload / MB_TO_GB);
        fprintf(faturaFile, "Kota aşım ücreti : %.2f TL\n", kotaAsimUcreti);

        // Ekstra verileri ekliyoruz
        printf("%s 2012 Faturası:\n", months[monthIndex]);
        printf("Toplam yükleme (upload) : %.2f GB\n", totalUpload / MB_TO_GB);
        printf("Toplam indirme (download) : %.2f GB\n", totalDownload / MB_TO_GB);
        printf("Kota aşım ücreti : %.2f TL\n", kotaAsimUcreti);

        // Dosyayı kapatıyoruz
        fclose(faturaFile);

        // Başka bir ay girmek ister misiniz sorusunu değiştiriyoruz
        char tekrar;
        printf("\nBaşka bir ay girmek ister misiniz? (Evet: e, Hayır: h): ");
        scanf(" %c", &tekrar);
        if (tekrar == 'h' || tekrar == 'H') {
            printf("Çıkılıyor...\n");
            break;  // Döngüyü sonlandırır
        }
        // Temizleme işlemi
        getchar();  // Kullanıcının önceki girişini temizlemek için
    }

    return 0;
}
