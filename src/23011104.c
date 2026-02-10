#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 50

// ogrenci ekleniyor, siliniyor ve guncelleniyor
typedef struct ogrenci
{
    char *ad;
    char *soyad;
    int puan;
    int id;
    int teslim;           // teslim ettiyse 0 degeri, etmediyse 1 degerini alir.
    struct ogrenci *next; // sonraki ogrenci
    struct ogrenci *prev; // onceki ogrenci
} Ogrenci;

// Yazar ad, soyad ve id
typedef struct yazar
{
    char *ad;
    char *soyad;
    int id;
    struct yazar *next;
} Yazar;

typedef struct aynikitap
{
    char *ISBN; // ISBN_1, 2, 3..

    char *rafDurumu;
    int zamanindaTeslim; // zamanindaysa 0, değilse 1
    struct aynikitap *next;
} AyniKitap;

// struct icinde struct kullanarak ayni kitaplari da linkli listeye aktardim.
typedef struct kitap
{
    char *kitapAd;
    char *ISBN;
    int adet;
    struct kitap *next;
    AyniKitap *bilgi; // ayni kitaplarin bilgisini tutan struct icinde struct
} Kitap;
// kitap yazar fonksiyonu
typedef struct kitapyazar
{
    char *ISBN;
    int id;
    struct kitapyazar *next;
} KitapYazar;

// odunc alma, teslim etme fonksiyonu.
typedef struct odunc
{
    char *ISBN;       // kitap ISBN 123456789011_1
    int id;           // kitabı alan ogrenci ogrenci id'si
    int durum;        // 0 ya da 1, 0 odunc| 1 teslim
    int gun, ay, yil; // sirasiyla gun ay yil aldim.
    struct odunc *next;
    struct odunc *prev;
} Odunc;

// Basta zaten CSV dosyasinin icinde olanlari okumak icin bir fonksiyon pointer
void CSVYukle(void (*fonksiyon)(void **, char *), void **head, char *dosyaAdi);
void OduncCSV(Odunc **head, char *dosya_adi);
void KitapYazarCSV(KitapYazar **head, char *dosya_adi);
void KitaplarCSV(Kitap **head, char *dosya_adi);
void YazarlarCSV(Yazar **head, char *dosya_adi);
void OgrencilerCSV(Ogrenci **head, char *dosya_adi);

void printMenu();

// OGRENCI FONKSIYONLARI
Ogrenci *createNode();
void addLast(Ogrenci **head, char *);
void deleteNode(Ogrenci **head, int del);
void updateNode(Ogrenci *head, int up, char *dosya_adi);
void CezaliOgrencileriListele(Ogrenci *head);
void KitapTeslimEtmeyenOgrenciler(Ogrenci *head);
void printOgrenciMenu();

void OgrenciSil(int silinecek_id, char *dosya_adi);
void OgrenciGoruntule(Ogrenci *head, int ID, Odunc *headOdunc);
void TumOgrencileriListele(Ogrenci *head, Odunc *headOdunc);

// YAZAR FONKSIYONLARI
void printYazarMenu();
Yazar *createNodeYazar();
void addLastYazar(Yazar **head, char *dosya_adi);
void YazarSil(int del, char *dosya_adi1, KitapYazar **headKY, Yazar **head, char *dosya_adi2);
void updateNodeYazar(Yazar *head, int up, char *dosya_adi); // burayı isme gore yap id degil
void YazarGoruntule(Yazar *head, char *ad, char *soyad, KitapYazar *headKitYaz, Kitap *headKit);

// KITAP FONKSIYONLARI
AyniKitap *createNodeAyniKitap();
void addLastKitap(Kitap **head, char *dosya_adi);
void KitapMenu();
void updateNodeKitap(Kitap *head, Odunc *hOdunc, KitapYazar *headKY, char *arananISBN, char *dosya_adi);
void KitapGoruntule(Kitap *head, char *arananISBN);
void TumKitaplariGoruntule(Kitap *head);
void KitapSil(char *del, char *dosya_adi, KitapYazar *headKY, Odunc *headOdunc, Kitap **headKit);
void KitapListele(Kitap *head);
void TeslimEdilmeyenKitap(Kitap *head);
void KitapYazarGuncelle(KitapYazar **head, Kitap *headKit, Yazar *, char *str, char *);

// KITAP YAZAR ESLESTIRM FONKSIYONLARI
void addLastKitapYazar(KitapYazar **head, char *dosya_adi, Kitap *kitap, Yazar *yazar);
KitapYazar *createNodeKitYaz();

// KITAP ODUNC ALMA ISLEMLERI
void addLastOdunc(Odunc **head, char *dosya_adi, Kitap **headKit, Ogrenci **headOgr, char *, char *);
Odunc *createNodeOdunc();



// FREE FONKSIYONLARI
void freeAllData(Ogrenci *ogrenciHead, Yazar *yazarHead, Kitap *kitapHead,KitapYazar *kitapYazarHead, Odunc *oduncHead);
void freeOduncList(Odunc *head);
void freeKitapYazarList(KitapYazar *head);
void freeKitapList(Kitap *head);
void freeKitap(Kitap *kitap);
void freeYazarList(Yazar *head);
void freeOgrenciList(Ogrenci *head);
void freeOgrenci(Ogrenci *ogrenci);
void freeAyniKitapList(AyniKitap *head);


int main()
{
    int secim;
    int secim2;
    int sil_id, id;
    int up;
    char *str;
    char *str1;
    char *searchISBN;
    char *del;
    str = (char *)malloc(sizeof(char) * MAX);
    str1 = (char *)malloc(sizeof(char) * MAX);
    searchISBN = (char *)malloc(sizeof(char) * MAX);
    del = (char *)malloc(sizeof(char) * MAX);
    KitapYazar *headKitYaz = NULL;
    Ogrenci *headOgr = NULL;
    Yazar *headYaz = NULL;
    Kitap *headKit = NULL;
    Odunc *headOdunc = NULL;

    CSVYukle((void (*)(void **, char *))OgrencilerCSV, (void **)&headOgr, "Ogrenciler.csv");
    CSVYukle((void (*)(void **, char *))YazarlarCSV, (void **)&headYaz, "yazarlar.csv");
    CSVYukle((void (*)(void **, char *))KitaplarCSV, (void **)&headKit, "kitap.csv");
    CSVYukle((void (*)(void **, char *))KitapYazarCSV, (void **)&headKitYaz, "KitapYazar.csv");
    CSVYukle((void (*)(void **, char *))OduncCSV, (void **)&headOdunc, "KitapOdunc.csv");

    printMenu();
    do
    {
        printf("Lutfen yapmak istediginiz islemi tuslayiniz.\n");
        scanf("%d", &secim);
        if (secim == 1)
        {

            printYazarMenu();
            do
            {
                printf("Lutfen yapmak istediginiz islemi tuslayiniz.\n");
                scanf("%d", &secim2);
                if (secim2 == 1)
                {
                    addLastYazar(&headYaz, "yazarlar.csv");
                }
                if (secim2 == 2)
                {
                    printf("Lutfen silinecek yazarin id'sini giriniz: ");
                    scanf("%d", &sil_id);
                    YazarSil(sil_id, "yazarlar.csv", &headKitYaz, &headYaz, "KitapYazar.csv");
                }
                if (secim2 == 3)
                {
                    printf("Lutfen guncellenecek yazarin id'sini giriniz: ");
                    scanf("%d", &up);
                    updateNodeYazar(headYaz, up, "yazarlar.csv");
                }
                if (secim2 == 4)
                {
                    printf("Lutfen goruntulenecek yazarin adini ve soyadini giriniz.\n");
                    scanf("%s %s", str, str1);
                    YazarGoruntule(headYaz, str, str1, headKitYaz, headKit);
                }
                printYazarMenu();
            } while (secim2 != 0);
        }

        else if (secim == 2)

        {
            printOgrenciMenu();

            do
            {
                printf("Lutfen yapmak istediginiz islemi tuslayiniz.\n");
                scanf("%d", &secim2);
                if (secim2 == 1)
                {
                    addLast(&headOgr, "Ogrenciler.csv");
                }
                else if (secim2 == 2)
                {
                    printf("Lutfen silinecek ogrencinin id'sini giriniz: ");
                    scanf("%d", &sil_id);
                    deleteNode(&headOgr, sil_id);
                    OgrenciSil(sil_id, "Ogrenciler.csv");
                }
                else if (secim2 == 3)
                {
                    printf("Lutfen guncellenecek ogrencinin id'sini giriniz: ");
                    scanf("%d", &up);
                    updateNode(headOgr, up, "Ogrenciler.csv");
                }
                else if (secim2 == 4)
                {
                    printf("Lutfen goruntulenecek ogrencinin ID'sini giriniz.\n");
                    scanf("%d", &id);
                    OgrenciGoruntule(headOgr, id, headOdunc);
                }
                else if (secim2 == 5)
                {
                    KitapTeslimEtmeyenOgrenciler(headOgr);
                }
                else if (secim2 == 6)
                {
                    CezaliOgrencileriListele(headOgr);
                }
                else if (secim2 == 7)
                {
                    TumOgrencileriListele(headOgr, headOdunc);
                }

                else if (secim2 == 8)
                {

                    addLastOdunc(&headOdunc, "KitapOdunc.csv", &headKit, &headOgr, "Ogrenciler.csv", "kitap.csv");
                }
                printOgrenciMenu();
            } while (secim2 != 0);
        }
        else if (secim == 3)
        {
            KitapMenu();
            do
            {
                printf("Lutfen secim giriniz: ");
                scanf("%d", &secim2);
                if (secim2 == 1)
                {
                    addLastKitap(&headKit, "kitap.csv");
                }
                if (secim2 == 2)
                {
                    printf("Lutfen Silmek istediginiz kitabin ISBN numarasini giriniz.\n");
                    scanf("%s", del);
                    KitapSil(del, "kitap.csv", headKitYaz, headOdunc, &headKit);
                }
                if (secim2 == 3)
                {
                    printf("Lutfen guncellenecek kitabin ISBN'sini girin: ");
                    scanf("%s", searchISBN);
                    updateNodeKitap(headKit, headOdunc, headKitYaz, searchISBN, "kitap.csv");
                }
                if (secim2 == 4)
                {
                    printf("Lutfen goruntulenecek kitabin adini giriniz: ");
                    scanf(" %[^\n]", searchISBN);
                    KitapGoruntule(headKit, searchISBN);
                }
                if (secim2 == 5)
                {
                    printf("Raftaki butun kitaplar gorun.\n");
                    KitapListele(headKit);
                }
                if (secim2 == 6)
                {
                    TeslimEdilmeyenKitap(headKit);
                }
                if (secim2 == 7)
                {
                    printf("Lutfen kitap-yazar eslestirmesi yapiniz.\n");
                    addLastKitapYazar(&headKitYaz, "KitapYazar.csv", headKit, headYaz);
                }
                if (secim2 == 8)
                {
                    printf("Lutfen guncellenecek Kitap-Yazar icin kitabin adini giriniz.\n");
                    scanf(" %[^\n]", str);
                    KitapYazarGuncelle(&headKitYaz, headKit, headYaz, str, "KitapYazar.csv");
                }
                KitapMenu();
            } while (secim2 != 0);
        }
        printMenu();
    } while (secim != 0);

    freeAllData(headOgr, headYaz, headKit, headKitYaz, headOdunc);
    return 0;
} // ----------------- END MAIN ------------------------------

// MENU
void printMenu()
{
    printf("\nYazar islemleri - 1 \n");
    printf("Ogrenci islemleri - 2\n");
    printf("Kitap islemleri - 3\n");
    printf("Programi Kapatma - 0\n\n");
}

void printOgrenciMenu()
{
    printf("\nOgrenci Ekle- 1\n"); // Ogrenci eklenecek dosyaya.
    printf("Ogrenci Silme- 2\n");
    printf("Ogrenci Guncelleme - 3\n");
    printf("Ogrenci Bilgisini Goster-4\n");
    printf("Kitap Teslim etmemis Ogrencileri Listele-5\n");
    printf("Cezali Ogrencileri Listele - 6\n");
    printf("Tum Ogrencileri Listele-7\n");
    printf("Kitap Odunc Al-Teslim Et-8\n");
    printf("Ana Menuye Donus: - 0\n");
}

void OgrenciSil(int silinecek_id, char *dosya_adi)
{
    FILE *dosya = fopen(dosya_adi, "r");
    FILE *gecici = fopen("temp.csv", "w");

    if (dosya == NULL || gecici == NULL)
    {
        printf("Dosya acilamadi!\n");
        return;
    }

    int id, puan;
    char ad[MAX], soyad[MAX];

    // Her satırı oku ve ID eşleşmiyorsa geçici dosyaya yaz
    while (fscanf(dosya, "%d,%[^,],%[^,],%d\n", &id, ad, soyad, &puan) == 4)
    {
        if (id != silinecek_id)
        {
            fprintf(gecici, "%d,%s,%s,%d\n", id, ad, soyad, puan);
        }
    }

    fclose(dosya);
    fclose(gecici);

    remove(dosya_adi);             // Eski dosyayı sil
    rename("temp.csv", dosya_adi); // Temp dosyasını eski adla değiştir
}

// OGRENCI CIFT YONLU LINKLI LIST ISLEMLERI

Ogrenci *createNode()
{

    Ogrenci *n;
    n = (Ogrenci *)malloc(sizeof(Ogrenci));
    n->next = NULL;
    n->ad = (char *)malloc(sizeof(char) * MAX);
    n->soyad = (char *)malloc(sizeof(char) * MAX);
    return n;
}

void KitapTeslimEtmeyenOgrenciler(Ogrenci *head)
{
    int bulundu = 0;
    printf("Kitap Teslim Etmeyen Ogrenciler Listeleniyor..\n");
    while (head != NULL)
    {
        if (head->teslim != 0)
        {
            bulundu = 1;
            printf("ad: %s, soyad: %s, id: %d\n", head->ad, head->soyad, head->id);
        }
        head = head->next;
    }
    if (bulundu == 0)
    {
        printf("Kitap Teslim Etmeyen Ogrenci Yok!\n");
    }
}

// linked listin sonuna eleman ekledim
void addLast(Ogrenci **head, char *dosya_adi)
{
    Ogrenci *newNode = createNode();

    if (*head == NULL)
    {
        *head = newNode;
        newNode->prev = NULL;
    }
    else
    {
        Ogrenci *temp = *head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newNode;
        newNode->prev = temp;
    }

    printf("ID giriniz: ");
    scanf("%d", &newNode->id);
    printf("Ad giriniz: ");
    scanf("%s", newNode->ad);
    printf("Soyad giriniz: ");
    scanf("%s", newNode->soyad);
    newNode->puan = 100;
    newNode->teslim = 0;

    FILE *dosya;

    dosya = fopen(dosya_adi, "a"); // append modda aç

    if (dosya == NULL)
    {
        printf("Dosya açilamadi!\n");
        return;
    }

    // ogrenci verisini ekle
    fprintf(dosya, "%d,%s,%s,%d\n", newNode->id, newNode->ad, newNode->soyad, newNode->puan);
    fclose(dosya);
}

// Linked List'ten NODE sildim.
void deleteNode(Ogrenci **head, int del)
{
    if (*head == NULL)
    {
        printf("Liste bos, silinecek bir sey yok!\n");
        return;
    }
    Ogrenci *current = *head;
    if (current->id == del)
    {
        *head = current->next;
        if (*head != NULL)
        {
            (*head)->prev = NULL;
        }
        free(current);
        printf("%d id'li ogrenci silindi.\n", del);
        return;
    }
    while (current != NULL && current->id != del)
    {
        current = current->next;
    }

    if (current == NULL)
    {
        printf("BOYLE BIR OGRENCI YOK\n");
        return;
    }

    if (current->prev != NULL)
    {
        current->prev->next = current->next;
    }

    if (current->next != NULL)
    {
        current->next->prev = current->prev;
    }

    free(current);
    printf("%d id'li ogrenci silindi.\n", del);
}

void updateNode(Ogrenci *head, int up, char *dosya_adi)
{
    Ogrenci *current = head;
    int bulundu = 0;

    while (current != NULL)
    {
        if (current->id == up && bulundu == 0)
        {
            printf("%d ID'li ogrencinin bilgilerini guncelleyiniz. (yeni id, ad, soyad, puan)\n", current->id);
            scanf("%d %s %s %d", &current->id, current->ad, current->soyad, &current->puan);
            bulundu = 1;
            printf("Ogrenci bilgileri guncellendi.\n");
        }
        current = current->next;
    }

    if (!bulundu)
    {
        printf("BOYLE BIR OGRENCI YOK\n");
        return;
    }

    // --- CSV GÜNCELLEME ---

    FILE *dosya = fopen(dosya_adi, "w");
    if (dosya == NULL)
    {
        printf("Dosya acilamadi!\n");
        return;
    }

    Ogrenci *tmp = head;
    while (tmp != NULL)
    {
        fprintf(dosya, "%d,%s,%s,%d\n", tmp->id, tmp->ad, tmp->soyad, tmp->puan);
        tmp = tmp->next;
    }
    fclose(dosya);
}

void OgrenciGoruntule(Ogrenci *head, int ID, Odunc *headOdunc)
{
    while (head != NULL && head->id != ID)
    {
        head = head->next; // yanlış olan "head->next = head;" satırı burada düzeltildi
    }

    if (head == NULL)
    {
        printf("BU ID'de ogrenci yok!\n");
    }
    else
    {
        printf("ID: %d, ad: %s, soyad: %s, puan: %d\n", head->id, head->ad, head->soyad, head->puan);
        while (headOdunc != NULL)
        {
            if (headOdunc->id == head->id)
            { // eger idler esitse:
                if (headOdunc->durum == 0)
                {
                    printf("Bu ogrenci %d.%d.%d tarihinde %s ISBN'li kitabi odunc almistir.\n", headOdunc->gun, headOdunc->ay, headOdunc->yil, headOdunc->ISBN);
                }
                else
                {
                    printf("Bu ogrenci %d.%d.%d tarihinde %s ISBN'li kitabi teslim etmistir\n", headOdunc->gun, headOdunc->ay, headOdunc->yil, headOdunc->ISBN);
                }
            }
            headOdunc = headOdunc->next;
        }
    }
}

void CezaliOgrencileriListele(Ogrenci *head)
{
    printf("Cezali Ogrenciler:\n");

    int cezali_var = 0; // Cezalı ogrenci olup olmadığını kontrol etmek için

    while (head != NULL)
    {
        // Puanı 100'den küçük olanlar cezalı ogrencilerdir
        if (head->puan < 100)
        {
            printf("ID: %d, Ad: %s, Soyad: %s, Puan: %d\n",
                   head->id, head->ad, head->soyad, head->puan);
            cezali_var = 1;
        }
        head = head->next;
    }

    if (!cezali_var)
    {
        printf("Cezali ogrenci bulunmamaktadir.\n");
    }
}

void TumOgrencileriListele(Ogrenci *head, Odunc *headOdunc)
{
    printf("Tum Ogrenciler Listeleniyor...\n");

    if (head == NULL)
    {
        printf("Kayitli ogrenci Yoktur.\n");
        return;
    }

    while (head != NULL)
    {
        printf("ID: %d, Ad: %s, Soyad: %s, Puan: %d\n", head->id, head->ad, head->soyad, head->puan);

        head = head->next;
    }
}
/*   =======================YAZAR ISLEMLERI====================== */

void printYazarMenu()
{
    printf("\nYazar Ekle- 1\n"); // Yazar eklenecek dosyaya.
    printf("Yazar Silme- 2\n");
    printf("Yazar Guncelleme - 3\n");
    printf("Yazar Bilgisini Goster-4\n");
    printf("Ana Menuye Don - 0\n");
}

Yazar *createNodeYazar()
{
    Yazar *n;
    n = (Yazar *)malloc(sizeof(Yazar));
    n->next = NULL;
    n->ad = (char *)malloc(sizeof(char) * MAX);
    n->soyad = (char *)malloc(sizeof(char) * MAX);
    return n;
}

// linkli listenin sonuna eleman ekledim - yazar
void addLastYazar(Yazar **head, char *dosya_adi)
{
    Yazar *newNode = createNodeYazar();
    Yazar *temp = *head;

    int id = 1;
    // ID belirleme: Listenin en sonundaki node'un id'sine göre
    while (temp != NULL)
    {
        id = temp->id + 1;
        temp = temp->next;
    }
    newNode->id = id;

    printf("Ad giriniz: ");
    scanf("%s", newNode->ad);
    printf("Soyad giriniz: ");
    scanf("%s", newNode->soyad);

    // Listeye ekleme
    if (*head == NULL)
    {
        *head = newNode;
    }
    else
    {
        temp = *head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    // Dosyaya yazma
    FILE *dosya = fopen(dosya_adi, "a");
    if (dosya == NULL)
    {
        printf("Dosya acilamadi!\n");
        return;
    }
    fprintf(dosya, "%d,%s,%s\n", newNode->id, newNode->ad, newNode->soyad);
    fclose(dosya);

    printf("Yazar eklendi!.\n");
}

void YazarSil(int del, char *dosya_adi1, KitapYazar **headKY, Yazar **head, char *dosya_adi2)
{
    Yazar *temp, *current;
    int bulundu = 0;

    // Yazarı listeden sil
    if (*head != NULL && (*head)->id == del)
    {
        temp = *head;
        *head = (*head)->next;
        free(temp);
        bulundu = 1;
    }
    else
    {
        current = *head;
        while (current != NULL && current->next != NULL)
        {
            if (current->next->id == del)
            {
                temp = current->next;
                current->next = current->next->next;
                free(temp);
                bulundu = 1;
                break;
            }
            current = current->next;
        }
    }

    if (!bulundu)
    {
        printf("BÖYLE BİR YAZAR YOK!\n");
        return;
    }
    else
    {
        printf("%d numarali yazar silindi.\n", del);
    }

    // KitapYazar eşleştirmelerinde id == del olanları -1 yap
    KitapYazar *currKY = *headKY;
    while (currKY != NULL)
    {
        if (currKY->id == del)
            currKY->id = -1;
        currKY = currKY->next;
    }

    // Yazar dosyasını güncelle (dosya_adi1)
    FILE *dosya1 = fopen(dosya_adi1, "w");
    if (dosya1 == NULL)
    {
        printf("Yazar dosyasi acilamadi!\n");
        return;
    }

    Yazar *tempY = *head;
    while (tempY != NULL)
    {
        fprintf(dosya1, "%d,%s,%s\n", tempY->id, tempY->ad, tempY->soyad);
        tempY = tempY->next;
    }
    fclose(dosya1);

    // KitapYazar dosyasını güncelle (dosya_adi2)
    FILE *dosya2 = fopen(dosya_adi2, "w");
    if (dosya2 == NULL)
    {
        printf("KitapYazar dosyasi acilamadi!\n");
        return;
    }

    KitapYazar *tempKY = *headKY;
    while (tempKY != NULL)
    {
        fprintf(dosya2, "%s,%d\n", tempKY->ISBN, tempKY->id);
        tempKY = tempKY->next;
    }
    fclose(dosya2);
}

void updateNodeYazar(Yazar *head, int up, char *dosya_adi)
{
    Yazar *current = head;
    int bulundu = 0;
    int flag = 0;

    while (current != NULL && bulundu == 0)
    {
        if (current->id == up)
        {
            printf("%d ID'li Yazarin bilgilerini guncelleyiniz. (yeni id, ad, soyad)\n", current->id);
            scanf("%d %s %s", &current->id, current->ad, current->soyad);
            Yazar *temp = head;

            while (temp != NULL && flag == 0)
            {
                if (temp != current && temp->id == current->id)
                {
                    flag = 1;
                    printf("GIRDIGINIZ ID ZATEN KULLANILIYOR. YENI BIR ID GIRINIZ: ");
                    scanf("%d", &current->id);
                    temp = head;
                    flag = 0;
                }
                else
                {
                    temp = temp->next;
                }
            }

            bulundu = 1;
            printf("Yazar bilgileri guncellendi.\n");
        }
        current = current->next;
    }

    if (!bulundu)
    {
        printf("BOYLE BIR YAZAR YOK!\n");
        return;
    }

    // --- CSV GÜNCELLEME ---

    FILE *dosya = fopen(dosya_adi, "w");
    if (dosya == NULL)
    {
        printf("Dosya acilamadi!\n");
        return;
    }

    Yazar *tmp = head;
    while (tmp != NULL)
    {
        fprintf(dosya, "%d,%s,%s\n", tmp->id, tmp->ad, tmp->soyad);
        tmp = tmp->next;
    }
    fclose(dosya);
}

void YazarGoruntule(Yazar *head, char *ad, char *soyad, KitapYazar *headKitYaz, Kitap *headKit)
{
    // Yazarı bul
    while (head != NULL && strcmp(ad, head->ad) != 0 && strcmp(soyad, head->soyad) != 0)
    {
        head = head->next;
    }

    if (head == NULL)
    {
        printf("Bu isme sahip yazar yok!\n");
    }
    else
    {
        printf("ID: %d, ad: %s, soyad: %s\n", head->id, head->ad, head->soyad);
        int bulundu = 0;

        KitapYazar *kitYazPtr = headKitYaz;
        while (kitYazPtr != NULL)
        {
            if (kitYazPtr->id == head->id)
            {
                Kitap *kitapPtr = headKit;
                while (kitapPtr != NULL)
                {
                    if (strcmp(kitapPtr->ISBN, kitYazPtr->ISBN) == 0)
                    {
                        printf("Kitap adi: %s, ISBN: %s, adet: %d\n", kitapPtr->kitapAd, kitapPtr->ISBN, kitapPtr->adet);
                        bulundu = 1;
                        break; // Aynı ISBN bir kez listelenir
                    }
                    kitapPtr = kitapPtr->next;
                }
            }
            kitYazPtr = kitYazPtr->next;
        }

        if (bulundu == 0)
        {
            printf("Bu yazar herhangi bir kitapla eslesmemis!\n");
        }
    }
}

// ================================== KITAP ISLEMLERI =========================

void KitapMenu()
{
    printf("\nKitap Ekle - 1\n");
    printf("Kitap Sil - 2\n");
    printf("Kitap Guncelle - 3\n");
    printf("Kitap Bilgisi Goruntule - 4\n");
    printf("Raftaki Kitaplari Listele - 5\n");
    printf("Zamaninda Teslim edilmeyen kitaplari listele - 6\n");
    printf("Kitap-Yazar Eslestir - 7\n");
    printf("Kitap Yazarini Guncelle - 8\n");
    printf("Ana Menuye Donus: - 0\n");
}
Kitap *createNodeKitap()
{
    Kitap *n;
    n = (Kitap *)malloc(sizeof(Kitap));
    n->next = NULL;
    n->ISBN = (char *)malloc(sizeof(char) * 14);
    n->kitapAd = (char *)malloc(sizeof(char) * MAX);
    return n;
}

void addLastKitap(Kitap **head, char *dosya_adi)
{
    Kitap *newNode = createNodeKitap();
    int i;

    printf("Kitabin adini giriniz: ");
    scanf(" %[^\n]", newNode->kitapAd);
    printf("Kitabin 13 Haneli ISBN numarasini giriniz: ");
    scanf("%s", newNode->ISBN);
    printf("Kitabin adet bilgisini giriniz: ");
    scanf("%d", &newNode->adet);

    // Ana Kitap linked listine ekle
    if (*head == NULL)
    {
        *head = newNode;
    }
    else
    {
        Kitap *temp = *head;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    // Bu kitabın AyniKitap listesini oluştur
    newNode->bilgi = NULL; // Başlangıçta boş

    for (i = 1; i <= newNode->adet; i++)
    {
        char str[10];
        AyniKitap *AyniNewNode = createNodeAyniKitap();
        sprintf(str, "%d", i);

        strcpy(AyniNewNode->ISBN, newNode->ISBN);
        strcat(AyniNewNode->ISBN, "_");
        strcat(AyniNewNode->ISBN, str);
        AyniNewNode->zamanindaTeslim = 0; /* KITABIN ZAMANINDA TESLIM EDILME DURUMU */
        strcpy(AyniNewNode->rafDurumu, "Rafta");

        // Bu kitabın kendi bilgi listesine ekle
        if (newNode->bilgi == NULL)
        {
            newNode->bilgi = AyniNewNode;
        }
        else
        {
            AyniKitap *temp = newNode->bilgi;
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
            temp->next = AyniNewNode;
        }
    }

    // Dosyaya yazma
    FILE *dosya = fopen(dosya_adi, "a");
    if (dosya == NULL)
    {
        printf("DOSYA ACILAMADI\n");
        return;
    }

    fprintf(dosya, "%s,%s,%d\n", newNode->kitapAd, newNode->ISBN, newNode->adet);

    AyniKitap *temp = newNode->bilgi;
    while (temp != NULL)
    {
        fprintf(dosya, "%s,%s\n", temp->ISBN, temp->rafDurumu);
        temp = temp->next;
    }

    fclose(dosya);
}

AyniKitap *createNodeAyniKitap()
{
    AyniKitap *n;
    n = (AyniKitap *)malloc(sizeof(AyniKitap));
    n->next = NULL;
    n->ISBN = (char *)malloc(sizeof(char) * MAX);
    n->rafDurumu = (char *)malloc(sizeof(char) * MAX);
    return n;
}

void KitapSil(char *del, char *dosya_adi, KitapYazar *headKY, Odunc *headOdunc, Kitap **headKit)
{
    // once silinecek kitabı bul
    Kitap *temp = *headKit;
    while (temp != NULL && strcmp(del, temp->ISBN) != 0)
    {
        temp = temp->next;
    }

    // kitap bulunamadıysa
    if (temp == NULL)
    {
        printf("BU ISBN'DE BIR KITAP BULUNAMADI\n");
        return;
    }

    // Kitap-yazar eşleştirmesi kontrolü
    KitapYazar *tempKY = headKY;
    while (tempKY != NULL && strcmp(tempKY->ISBN, temp->ISBN) != 0)
    {
        tempKY = tempKY->next;
    }
    if (tempKY != NULL && tempKY->id != -1)
    {
        printf("BU KITAP SU ANDA BIR YAZARLA ESLESMIS DURUMDA OLDUGU ICIN SILINEMEZ!\n");
        return;
    }

    // Tüm kopyaların rafta olup olmadığını kontrol et
    AyniKitap *temp2 = temp->bilgi;
    while (temp2 != NULL)
    {
        if (strcmp(temp2->rafDurumu, "Rafta") != 0)
        {
            printf("%s ISBNLI KITAP SU ANDA RAFTA OLMADIGI ICIN SILINEMEDI!\n", temp2->ISBN);
            return;
        }
        temp2 = temp2->next;
    }

    // Kitabı sil
    if (*headKit != NULL && strcmp((*headKit)->ISBN, del) == 0)
    {
        // Head node'u sil
        Kitap *nodeToDelete = *headKit;
        *headKit = (*headKit)->next;

        // AyniKitap node'larını serbest bırak
        AyniKitap *temp2 = nodeToDelete->bilgi;
        while (temp2 != NULL)
        {
            AyniKitap *temp3 = temp2;
            temp2 = temp2->next;
            free(temp3);
        }

        // Ana Kitap node'unu serbest bırak
        free(nodeToDelete);
        printf("KITAP BASARIYLA SILINDI\n");
    }
    else
    {
        // Head olmayan node'u sil - önceki node'u bul
        Kitap *prev = *headKit;
        while (prev->next != NULL && strcmp(prev->next->ISBN, del) != 0)
        {
            prev = prev->next;
        }

        if (prev->next != NULL)
        {
            Kitap *nodeToDelete = prev->next;
            prev->next = nodeToDelete->next;

            // AyniKitap node'larını serbest bırak
            AyniKitap *temp2 = nodeToDelete->bilgi;
            while (temp2 != NULL)
            {
                AyniKitap *temp3 = temp2;
                temp2 = temp2->next;
                free(temp3);
            }

            // Ana Kitap node'unu serbest bırak
            free(nodeToDelete);
            printf("KITAP BASARIYLA SILINDI\n");
        }
    }

    // Güncellenmiş listeyi dosyaya yaz
    FILE *dosya = fopen(dosya_adi, "w");
    if (dosya == NULL)
    {
        printf("DOSYA ACILAMADI\n");
        return;
    }

    Kitap *tmp = *headKit;
    while (tmp != NULL)
    {
        fprintf(dosya, "%s,%s,%d\n", tmp->kitapAd, tmp->ISBN, tmp->adet);
        AyniKitap *tmp1 = tmp->bilgi;
        while (tmp1 != NULL)
        {
            fprintf(dosya, "%s,%s\n", tmp1->ISBN, tmp1->rafDurumu);
            tmp1 = tmp1->next;
        }
        tmp = tmp->next;
    }

    fclose(dosya);
}

// BUNU KITAP ADINA GORE YAP - yapildi
void KitapGoruntule(Kitap *head, char *arananISBN)
{
    int i;
    while (head != NULL && strcmp(head->kitapAd, arananISBN) != 0)
    {
        head = head->next;
    }
    if (head != NULL)
    {
        printf("\nkitap ismi: %s, ISBN: %s, adet: %d\n", head->kitapAd, head->ISBN, head->adet);
        AyniKitap *kopya = head->bilgi;
        while (kopya != NULL)
        {
            printf("%s, %s\n", kopya->ISBN, kopya->rafDurumu);
            kopya = kopya->next;
        }
    }
}

// BURADA RAFTA OLANLARI CEVIR !!!!

void KitapListele(Kitap *head)
{
    int bulundu = 0;
    Kitap *current = head;
    while (current != NULL)
    {
        AyniKitap *temp;
        temp = current->bilgi;
        while (temp != NULL)
        {
            if (strncmp(temp->rafDurumu, "Rafta", 5) == 0)
            {
                printf("Kitap ismi : %s, ISBN: %s\n", current->kitapAd, temp->ISBN);
                bulundu = 1;
            }
            temp = temp->next;
        }
        current = current->next;
    }
    if (bulundu == 0)
    {
        printf("RAFTA KITAP YOK!.\n");
    }
}

void TeslimEdilmeyenKitap(Kitap *head)
{
    printf("Zamaninda Teslim Edilmeyen Kitaplar Listeleniyor...\n");
    while (head != NULL)
    {
        AyniKitap *curr = head->bilgi;
        while (curr != NULL)
        {
            if (curr->zamanindaTeslim == 1)
            {
                printf("Kitap Adi: %s, ISBN NO: %s\n", head->kitapAd, curr->ISBN);
            }
            curr = curr->next;
        }
        head = head->next;
    }
}

void TumKitaplariGoruntule(Kitap *head)
{
    while (head != NULL)
    {

        AyniKitap *kopya = head->bilgi;
        while (kopya != NULL)
        {
            if (strcmp(kopya->ISBN, "Rafta") != 0)
            {
                printf("Kitap ismi: %s, ISBN: %s\n", head->kitapAd, kopya->ISBN);
                kopya = kopya->next;
            }
        }

        head = head->next;
    }
}

// Kitap Guncelle
void updateNodeKitap(Kitap *head, Odunc *hOdunc, KitapYazar *headKY, char *arananISBN, char *dosya_adi)
{

    // Önce kitabı bul
    Kitap *temp = head;
    while (temp != NULL)
    {

        if (strcmp(arananISBN, temp->ISBN) == 0)
        {
            break;
        }
        temp = temp->next;
    }

    if (temp == NULL)
    {
        printf("BOYLE BIR KITAP YOK\n");
        return;
    }

    // Yazar kontrolü - Geçici pointer kullan
    KitapYazar *tempKY = headKY;
    while (tempKY != NULL)
    {
        if (strcmp(tempKY->ISBN, arananISBN) == 0)
        {
            printf("Bu kitap su anda bir yazarla eslesmis durumda! Guncellenemez.\n");
            return;
        }
        tempKY = tempKY->next;
    }

    // Ödünç kontrolü - Kitabın herhangi bir kopyası ödünçte mi?
    Odunc *tempOdunc = hOdunc;
    while (tempOdunc != NULL)
    {
        // Ödünç kaydındaki ISBN, aranan kitabın bir kopyası mı kontrol et
        // Örnek: arananISBN = "123456", ödünçteki = "123456_1"
        if (strstr(tempOdunc->ISBN, arananISBN) == tempOdunc->ISBN &&
            tempOdunc->durum == 0) // 0 = ödünçte
        {
            printf("Bu kitap su anda rafta degil! Guncellenemez.\n");
            return;
        }
        tempOdunc = tempOdunc->next;
    }

    // Eski değerleri sakla
    int eskiAdet = temp->adet;

    // Geçici buffer'lar oluştur (char* olduğu için)
    char yeniAd[200];
    char yeniISBN[50];
    int yeniAdet;

    printf("Lutfen Kitap bilgilerini guncelleyiniz.(kitap adi, ISBN, adet)\n");

    scanf(" %[^\n]", yeniAd);

    scanf(" %s", yeniISBN);

    scanf("%d", &yeniAdet);

    // Eski belleği serbest bırak ve yenilerini ata
    free(temp->kitapAd);
    free(temp->ISBN);

    temp->kitapAd = (char *)malloc(strlen(yeniAd) + 1);
    temp->ISBN = (char *)malloc(strlen(yeniISBN) + 1);

    if (temp->kitapAd == NULL || temp->ISBN == NULL)
    {
        printf("Bellek tahsis edilemedi!\n");
        return;
    }

    strcpy(temp->kitapAd, yeniAd);
    strcpy(temp->ISBN, yeniISBN);
    temp->adet = yeniAdet;

    // AyniKitap listesini güncelle
    if (temp->adet != eskiAdet)
    {
        // Tüm mevcut AyniKitap düğümlerini sil
        AyniKitap *bilgi = temp->bilgi;
        while (bilgi != NULL)
        {
            AyniKitap *silinecek = bilgi;
            bilgi = bilgi->next;

            // char* olduğu için free et
            free(silinecek->ISBN);
            free(silinecek->rafDurumu);
            free(silinecek);
        }
        temp->bilgi = NULL;

        // Yeni AyniKitap düğümlerini oluştur
        AyniKitap *son = NULL;
        int i;
        for ( i = 1; i <= temp->adet; i++)
        {
            AyniKitap *yeni = (AyniKitap *)malloc(sizeof(AyniKitap));
            if (yeni == NULL)
            {
                printf("Bellek tahsis edilemedi!\n");
                return;
            }

            // ISBN_i formatında string oluştur
            char isbnBuffer[100];
            sprintf(isbnBuffer, "%s_%d", temp->ISBN, i);

            yeni->ISBN = (char *)malloc(strlen(isbnBuffer) + 1);
            yeni->rafDurumu = (char *)malloc(strlen("Rafta") + 1);

            if (yeni->ISBN == NULL || yeni->rafDurumu == NULL)
            {
                printf("Bellek tahsis edilemedi!\n");
                free(yeni);
                return;
            }

            strcpy(yeni->ISBN, isbnBuffer);
            strcpy(yeni->rafDurumu, "Rafta");
            yeni->zamanindaTeslim = 0;
            yeni->next = NULL;

            if (temp->bilgi == NULL)
            {
                temp->bilgi = yeni;
            }
            else
            {
                son->next = yeni;
            }
            son = yeni;
        }
    }
    else
    {
        // Adet değişmediyse sadece ISBN'leri güncelle
        AyniKitap *bilgi = temp->bilgi;
        int i;
        for ( i = 1; i <= temp->adet && bilgi != NULL; i++)
        {
            // Eski ISBN'i serbest bırak
            free(bilgi->ISBN);

            // Yeni ISBN oluştur
            char isbnBuffer[100];
            sprintf(isbnBuffer, "%s_%d", temp->ISBN, i);

            bilgi->ISBN = (char *)malloc(strlen(isbnBuffer) + 1);
            if (bilgi->ISBN == NULL)
            {
                printf("Bellek tahsis edilemedi!\n");
                return;
            }
            strcpy(bilgi->ISBN, isbnBuffer);

            bilgi = bilgi->next;
        }
    }

    // Dosyaya güncellenmiş bilgileri yaz
    FILE *dosya = fopen(dosya_adi, "w");
    if (dosya == NULL)
    {
        printf("Dosya acilamadi!\n");
        return;
    }

    Kitap *tmp = head;
    while (tmp != NULL)
    {
        fprintf(dosya, "%s,%s,%d\n", tmp->kitapAd, tmp->ISBN, tmp->adet);

        AyniKitap *bilgi = tmp->bilgi;
        while (bilgi != NULL)
        {
            fprintf(dosya, "%s,%s\n", bilgi->ISBN, bilgi->rafDurumu);
            bilgi = bilgi->next;
        }
        tmp = tmp->next;
    }

    fclose(dosya);
    printf("Kitap bilgileri basariyla guncellendi.\n");
}
// ================ KITAP YAAZAR ESLESTRIME ==========

KitapYazar *createNodeKitYaz()
{
    KitapYazar *n;
    n = (KitapYazar *)malloc(sizeof(KitapYazar));
    n->next = NULL;
    n->ISBN = (char *)malloc(sizeof(char) * 14);
    return n;
}

void addLastKitapYazar(KitapYazar **head, char *dosya_adi, Kitap *kitap, Yazar *yazar)
{
    KitapYazar *newNode = createNodeKitYaz();
    char *str;
    str = (char *)malloc(sizeof(char) * MAX);
    int ID;

    printf("Kitap-Yazar Eslestirmek icin sirasiyla ISBN ve yazar id giriniz.\n");

    scanf("%s", str);

    // Kitap arama
    Kitap *kptr = kitap;
    while (kptr != NULL && strcmp(kptr->ISBN, str) != 0)
    {
        kptr = kptr->next;
    }
    if (kptr == NULL)
    {
        printf("BOYLE BIR KITAP YOK!\n");
        return;
    }
    strcpy(newNode->ISBN, str);

    // Yazar ID al
    scanf("%d", &ID);

    // Yazar arama
    Yazar *yptr = yazar;
    while (yptr != NULL && yptr->id != ID)
    {
        yptr = yptr->next;
    }
    if (yptr == NULL)
    {
        printf("BU ID'YE SAHIP BIR YAZAR YOK!\n");
        return;
    }
    newNode->id = ID;

    newNode->next = NULL;
    if (*head == NULL)
    {
        *head = newNode;
    }
    else
    {
        KitapYazar *temp = *head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = newNode;
    }

    FILE *dosya = fopen(dosya_adi, "a");
    if (dosya == NULL)
    {
        printf("DOSYA ACILAMADI\n");
        return;
    }
    fprintf(dosya, "%s,%d\n", newNode->ISBN, newNode->id);
    fclose(dosya);
    free(str);
    printf("Kitap-Yazar eslestirildi!.\n");
}

void KitapYazarGuncelle(KitapYazar **head, Kitap *headKit, Yazar *headYazar, char *str, char *dosya_adi)
{
    // Kitabı bul
    while (headKit != NULL && strcmp(headKit->kitapAd, str) != 0)
    {
        headKit = headKit->next;
    }

    if (headKit == NULL)
    {
        printf("Bu isimde bir kitap yok!\n");
        return;
    }

    // Mevcut kitap-yazar eşleşmesini bul
    KitapYazar *current = *head;
    while (current != NULL && strcmp(current->ISBN, headKit->ISBN) != 0)
    {
        current = current->next;
    }

    if (current == NULL)
    {
        printf("BU KITAP SU ANDA BIR YAZARLA ESLESMIS DURUMDA DEGIL!\n");
        return;
    }

    // Yeni yazar ID'sini al ve kontrol et
    int ID;
    printf("Lutfen bu kitabin yeni yazar ID'sini giriniz: ");
    if (scanf("%d", &ID) != 1)
    {
        printf("Gecersiz ID formati!\n");
        return;
    }

    // Yazarın var olup olmadığını kontrol et
    Yazar *tempYazar = headYazar;
    while (tempYazar != NULL && tempYazar->id != ID)
    {
        tempYazar = tempYazar->next;
    }

    if (tempYazar == NULL)
    {
        printf("Bu ID'ye sahip yazar bulunamadi!\n");
        return;
    }

    // Güncelleme yap
    current->id = ID;
    printf("Kitabin yazari guncellendi!\n");

    // Dosyaya kaydet
    FILE *dosya = fopen(dosya_adi, "w");
    if (dosya == NULL)
    {
        printf("DOSYA ACILAMADI\n");
        return;
    }

    KitapYazar *temp = *head;
    while (temp != NULL)
    {
        fprintf(dosya, "%s,%d\n", temp->ISBN, temp->id);
        temp = temp->next;
    }

    fclose(dosya);
}
// ============== OGRENCI ODUNC ALMA ISLEMLERI

Odunc *createNodeOdunc()
{
    Odunc *n;
    n = (Odunc *)malloc(sizeof(Odunc));
    n->next = NULL;
    n->prev = NULL;
    n->ISBN = (char *)malloc(sizeof(char) * MAX);
    return n;
}

void addLastOdunc(Odunc **head, char *dosya_adi, Kitap **headKit, Ogrenci **headOgr, char *dosya_adi_Ogr, char *dosya_adi_kit)
{
    Odunc *newNode = createNodeOdunc();
    Ogrenci *temp2 = *headOgr;
    char *str;
    str = (char *)malloc(MAX * sizeof(char));
    int ID;
    int flag = 0;
    int islem;
    int g, a, y;
    AyniKitap *bulunan_kitap = NULL; // Bulunan kitabı tutacak pointer

    printf("Lutfen Odunc-Teslim islemleri icin kitabin ISBN numarasini girin: ");
    scanf("%s", str);

    Kitap *temp = *headKit;
    while (temp != NULL && flag == 0)
    {
        AyniKitap *temp5 = temp->bilgi;
        while (temp5 != NULL && strcmp(temp5->ISBN, str) != 0)
        {
            temp5 = temp5->next;
        }
        if (temp5 != NULL)
        {
            flag = 1;
            bulunan_kitap = temp5; // Bulunan kitabı sakla

            printf("Lutfen ogrencinin ID'sini giriniz.\n");
            scanf("%d", &ID);
            while (temp2 != NULL && temp2->id != ID)
            {
                temp2 = temp2->next;
            }
            if (temp2 == NULL)
            {
                printf("BOYLE BIR OGRENCI YOK!");
                return;
            }
            else
            {
                if (temp2->puan < 0)
                {
                    printf("BU OGRENCIYE KITAP VERILEMEZ!\n");
                    return;
                }
                else
                {
                    printf("Islemi giriniz. Odunc-0 , Teslim-1: ");
                    scanf("%d", &islem);

                    if (islem == 0) // ÖDÜNÇ ALMA İŞLEMİ
                    {
                        // Kitabın zaten ödünç alınmış olup olmadığını kontrol et
                        if (strcmp(bulunan_kitap->rafDurumu, "Rafta") != 0)
                        {
                            printf("BU KITAP ZATEN ODUNC ALINMIS!\n");
                            return;
                        }

                        printf("Lutfen Tarih Giriniz. ");
                        printf("\nGun : ");
                        scanf("%d", &g);
                        printf("\nAy : ");
                        scanf("%d", &a);
                        printf("\nYil : ");
                        scanf("%d", &y);
                        temp2->teslim = 1;
                        char str2[9];
                        sprintf(str2, "%d", temp2->id);
                        strcpy(bulunan_kitap->rafDurumu, str2); // DÜZELTME: bulunan_kitap kullan
                    }
                    else if (islem == 1) // TESLİM ETME İŞLEMİ
                    {
                        // Kitabın ödünç alındığını kontrol et (durum = 0 olan kayıt bul)
                        Odunc *odunc_kaydi = NULL;
                        Odunc *current = *head;
                        int flag = 0;
                        while (current != NULL)
                        {
                            if (strcmp(current->ISBN, str) == 0 && current->durum == 0 && current->id == ID && flag == 0)
                            {
                                odunc_kaydi = current;
                                flag = 1;
                            }
                            current = current->next;
                        }

                        if (odunc_kaydi == NULL)
                        {
                            printf("BU KITAP BU OGRENCI TARAFINDAN ODUNC ALINMAMIS!\n");
                            return;
                        }

                        if (strcmp(bulunan_kitap->rafDurumu, "Rafta") == 0)
                        {
                            printf("BU KITAP ZATEN TESLIM EDILMIS !\n");
                            return;
                        }

                        printf("Lutfen Tarih Giriniz. ");
                        printf("\nGun : ");
                        scanf("%d", &g);
                        printf("\nAy : ");
                        scanf("%d", &a);
                        printf("\nYil : ");
                        scanf("%d", &y);

                        // Tarih kontrolü - teslim tarihi ödünç alma tarihinden sonra olmalı
                        int teslim_toplam = g + a * 30 + y * 365;
                        int odunc_toplam = odunc_kaydi->gun + odunc_kaydi->ay * 30 + odunc_kaydi->yil * 365;

                        if (teslim_toplam < odunc_toplam)
                        {
                            printf("TESLIM EDILEN TARIH ODUNC ALINAN TARIHDEN DAHA SONRA OLMALI!\n");
                            return;
                        }

                        // Geç teslim kontrolü (15 günden fazla)
                        int fark = (y - odunc_kaydi->yil) * 365 + (a - odunc_kaydi->ay) * 30 + (g - odunc_kaydi->gun);
                        if (fark > 15)
                        {
                            // ogrenciye 10 puan ceza ver
                            temp2->puan = temp2->puan - 10;
                            printf("GEC TESLIM NEDENIYLE 10 PUAN CEZA VERILDI!\n");
                            temp5->zamanindaTeslim = 1; // zamaninda teslim edilmedi.
                        }
                        temp2->teslim = 0;
                        // DÜZELTME: bulunan_kitap kullan
                        strcpy(bulunan_kitap->rafDurumu, "Rafta");
                    }
                    else
                    {
                        printf("GECERSIZ ISLEM! Lutfen 0 veya 1 giriniz.\n");
                        return;
                    }
                }
            }
        }
        temp = temp->next;
    }

    if (flag == 0)
    {
        printf("ISBN BULUNAMADI!\n");
        return;
    }

    // Yeni node'u doldur
    strcpy(newNode->ISBN, str);
    newNode->id = ID;
    newNode->durum = islem;
    newNode->ay = a;
    newNode->yil = y;
    newNode->gun = g;

    // Linkli listeye ekle
    if (*head == NULL)
    {
        *head = newNode;
    }
    else
    {
        Odunc *temp_odunc = *head;
        while (temp_odunc->next != NULL)
        {
            temp_odunc = temp_odunc->next;
        }
        temp_odunc->next = newNode;
        newNode->prev = temp_odunc;
    }

    // Dosyaya yaz
    FILE *dosya = fopen(dosya_adi, "a");
    if (dosya == NULL)
    {
        printf("DOSYA ACILAMADI\n");
        return;
    }
    fprintf(dosya, "%s,%d,%d,%d.%d.%d\n", newNode->ISBN, newNode->id, newNode->durum, newNode->gun, newNode->ay, newNode->yil);
    fclose(dosya);

    if (islem == 0)
    {
        printf("KITAP BASARIYLA ODUNC VERILDI!\n");
    }
    else
    {
        printf("KITAP BASARIYLA TESLIM ALINDI!\n");
    }

    // ogrenci dosyasını güncelle
    FILE *dosya1 = fopen(dosya_adi_Ogr, "w");
    if (dosya1 == NULL)
    {
        printf("Dosya acilamadi!\n");
        return;
    }

    Ogrenci *tmp = *headOgr;
    while (tmp != NULL)
    {
        fprintf(dosya1, "%d,%s,%s,%d\n", tmp->id, tmp->ad, tmp->soyad, tmp->puan);
        tmp = tmp->next;
    }
    fclose(dosya1);

    // Kitap dosyasını güncelle
    FILE *dosya3 = fopen(dosya_adi_kit, "w"); // "a" yerine "w" kullandım
    if (dosya3 == NULL)
    {
        printf("DOSYA ACILAMADI\n");
        return;
    }

    Kitap *temp5 = *headKit;
    while (temp5 != NULL)
    {
        fprintf(dosya3, "%s,%s,%d\n", temp5->kitapAd, temp5->ISBN, temp5->adet);
        AyniKitap *temp6 = temp5->bilgi;
        while (temp6 != NULL)
        {
            fprintf(dosya3, "%s,%s\n", temp6->ISBN, temp6->rafDurumu);
            temp6 = temp6->next;
        }
        temp5 = temp5->next; // Bu satır eksikti!
    }
    free(str);
    fclose(dosya3);
}

// ogrenci CSV dosyasını okuyup bagli listeye yukler
void OgrencilerCSV(Ogrenci **head, char *dosya_adi)
{
    FILE *dosya = fopen(dosya_adi, "r");
    if (dosya == NULL)
    {
        printf("Ogrenci dosyasi bulunamadi, yeni dosya olusturulacak.\n");
        return;
    }

    int id, puan;
    char *ad, *soyad;
    char *line;
    ad = (char *)malloc(sizeof(char) * 50);
    soyad = (char *)malloc(sizeof(char) * 50);
    line = (char *)malloc(sizeof(char) * 250);

    while (fgets(line, 250, dosya))
    {
        // Satırı parse et
        if (sscanf(line, "%d,%[^,],%[^,],%d", &id, ad, soyad, &puan) == 4)
        {
            // Yeni node oluştur
            Ogrenci *newNode = createNode();
            newNode->id = id;
            strcpy(newNode->ad, ad);
            strcpy(newNode->soyad, soyad);
            newNode->puan = puan;
            newNode->teslim = 0; // Varsayılan değer

            // bagli listeye ekle
            if (*head == NULL)
            {
                *head = newNode;
                newNode->prev = NULL;
            }
            else
            {
                Ogrenci *temp = *head;
                while (temp->next != NULL)
                {
                    temp = temp->next;
                }
                temp->next = newNode;
                newNode->prev = temp;
            }
        }
    }
    free(ad);
    free(soyad);
    free(line);
    fclose(dosya);
}

// Yazar CSV dosyasını okuyup bagli listeye yukler
void YazarlarCSV(Yazar **head, char *dosya_adi)
{
    FILE *dosya = fopen(dosya_adi, "r");
    if (dosya == NULL)
    {
        printf("Yazar dosyasi bulunamadi, yeni dosya olusturulacak.\n");
        return;
    }

    int id;
    char *ad, *soyad;
    char *line;
    ad = (char *)malloc(sizeof(char) * 50);
    soyad = (char *)malloc(sizeof(char) * 50);
    line = (char *)malloc(sizeof(char) * 250);

    while (fgets(line, 250, dosya))
    {
        if (sscanf(line, "%d,%[^,],%[^,]", &id, ad, soyad) == 3)
        {
            Yazar *newNode = createNodeYazar();
            newNode->id = id;
            strcpy(newNode->ad, ad);
            strcpy(newNode->soyad, soyad);

            // bagli listeye ekle
            if (*head == NULL)
            {
                *head = newNode;
            }
            else
            {
                Yazar *temp = *head;
                while (temp->next != NULL)
                {
                    temp = temp->next;
                }
                temp->next = newNode;
            }
        }
    }

    free(ad);
    free(soyad);
    free(line);
    fclose(dosya);
}

// Kitap CSV dosyasını okuyup bagli listeye yukler
void KitaplarCSV(Kitap **head, char *dosya_adi)
{
    FILE *dosya = fopen(dosya_adi, "r");
    if (dosya == NULL)
    {
        printf("Kitap dosyasi bulunamadi, yeni dosya olusturulacak.\n");
        return;
    }

    char *line;
    line = (char *)malloc(sizeof(char) * 500);
    Kitap *currentKitap = NULL;

    while (fgets(line, 500, dosya))
    {
        line[strcspn(line, "\n")] = 0;

        // Eğer satırda 3 virgül varsa bu ana kitap bilgisidir.
        int virgul_sayisi = 0;
        int i;
        for (i = 0; line[i]; i++)
        {
            if (line[i] == ',')
                virgul_sayisi++;
        }

        if (virgul_sayisi == 2)
        {
            // Ana kitap bilgisi
            char *kitapAd, *ISBN;
            ISBN = (char *)malloc(sizeof(char) * 20);
            kitapAd = (char *)malloc(sizeof(char) * 50);
            int adet;

            if (sscanf(line, "%[^,],%[^,],%d", kitapAd, ISBN, &adet) == 3)
            {
                Kitap *newKitap = createNodeKitap();
                strcpy(newKitap->kitapAd, kitapAd);
                strcpy(newKitap->ISBN, ISBN);
                newKitap->adet = adet;
                newKitap->bilgi = NULL;

                // Ana kitap listesine ekle
                if (*head == NULL)
                {
                    *head = newKitap;
                }
                else
                {
                    Kitap *temp = *head;
                    while (temp->next != NULL)
                    {
                        temp = temp->next;
                    }
                    temp->next = newKitap;
                }

                currentKitap = newKitap;
            }
            free(ISBN);
            free(kitapAd);
        }
        else if (virgul_sayisi == 1 && currentKitap != NULL)
        {
            // Kitap kopyası bilgisi.
            char *ISBN, *rafDurumu;
            ISBN = (char *)malloc(sizeof(char) * 20);
            rafDurumu = (char *)malloc(sizeof(char) * 50);

            if (sscanf(line, "%[^,],%s", ISBN, rafDurumu) == 2)
            {
                AyniKitap *newKopya = createNodeAyniKitap();
                strcpy(newKopya->ISBN, ISBN);
                strcpy(newKopya->rafDurumu, rafDurumu);
                newKopya->zamanindaTeslim = 0; // Varsayılan deger

                // Kitabın bilgi listesine ekledim
                if (currentKitap->bilgi == NULL)
                {
                    currentKitap->bilgi = newKopya;
                }
                else
                {
                    AyniKitap *temp = currentKitap->bilgi;
                    while (temp->next != NULL)
                    {
                        temp = temp->next;
                    }
                    temp->next = newKopya;
                }
            }
            free(ISBN);
            free(rafDurumu);
        }
    }
    free(line);
    fclose(dosya);
}

// KitapYazar CSV dosyasını okuyup  listeye yukler
void KitapYazarCSV(KitapYazar **head, char *dosya_adi)
{
    FILE *dosya = fopen(dosya_adi, "r");
    if (dosya == NULL)
    {
        printf("KitapYazar dosyasi bulunamadi, yeni dosya olusturulacak.\n");
        return;
    }

    char *ISBN;
    ISBN = (char *)malloc(sizeof(char) * 20);
    int id;
    char *line;
    line = (char *)malloc(sizeof(char) * 200);

    while (fgets(line, 200, dosya))
    {
        if (sscanf(line, "%[^,],%d", ISBN, &id) == 2)
        {
            KitapYazar *newNode = createNodeKitYaz();
            strcpy(newNode->ISBN, ISBN);
            newNode->id = id;

            //  listeye ekle
            if (*head == NULL)
            {
                *head = newNode;
            }
            else
            {
                KitapYazar *temp = *head;
                while (temp->next != NULL)
                {
                    temp = temp->next;
                }
                temp->next = newNode;
            }
        }
    }
    free(ISBN);
    free(line);
    fclose(dosya);
}

// Ödünç CSV dosyasını okuyup  listeye yukler

void OduncCSV(Odunc **head, char *dosya_adi)
{
    FILE *dosya = fopen(dosya_adi, "r");
    if (dosya == NULL)
    {
        printf("Odunc dosyasi bulunamadi, yeni dosya olusturulacak.\n");
        return;
    }

    char *ISBN;
    ISBN = (char *)malloc(sizeof(char) * 20);
    int id, durum, gun, ay, yil;
    char *line;
    line = (char *)malloc(sizeof(char) * 200);

    while (fgets(line, 200, dosya))
    {
        if (sscanf(line, "%[^,],%d,%d,%d.%d.%d", ISBN, &id, &durum, &gun, &ay, &yil) == 6)
        {
            Odunc *newNode = createNodeOdunc();
            strcpy(newNode->ISBN, ISBN);
            newNode->id = id;
            newNode->durum = durum;
            newNode->gun = gun;
            newNode->ay = ay;
            newNode->yil = yil;

            //  listeye ekle
            if (*head == NULL)
            {
                *head = newNode;
                newNode->prev = NULL;
            }
            else
            {
                Odunc *temp = *head;
                while (temp->next != NULL)
                {
                    temp = temp->next;
                }
                temp->next = newNode;
                newNode->prev = temp;
            }
        }
    }
    free(ISBN);
    free(line);
    fclose(dosya);
}

void CSVYukle(void (*fonksiyon)(void **, char *), void **head, char *dosyaAdi)
{
    fonksiyon(head, dosyaAdi);
}

void freeAyniKitapList(AyniKitap *head)
{
    AyniKitap *current = head;
    AyniKitap *next;

    while (current != NULL)
    {
        next = current->next;

        free(current->ISBN);
        free(current->rafDurumu);

        free(current);

        current = next;
    }
}

void freeOgrenci(Ogrenci *ogrenci)
{
    if (ogrenci != NULL)
    {
        free(ogrenci->ad);
        free(ogrenci->soyad);
        free(ogrenci);
    }
}

void freeOgrenciList(Ogrenci *head)
{
    Ogrenci *current = head;
    Ogrenci *next;

    while (current != NULL)
    {
        next = current->next;
        freeOgrenci(current);
        current = next;
    }
}

void freeYazarList(Yazar *head)
{
    Yazar *current = head;
    Yazar *next;

    while (current != NULL)
    {
        next = current->next;

        free(current->ad);
        free(current->soyad);

        free(current);

        current = next;
    }
}

void freeKitap(Kitap *kitap)
{
    if (kitap != NULL)
    {
        free(kitap->kitapAd);
        free(kitap->ISBN);

        freeAyniKitapList(kitap->bilgi);

        free(kitap);
    }
}

void freeKitapList(Kitap *head)
{
    Kitap *current = head;
    Kitap *next;

    while (current != NULL)
    {
        next = current->next;
        freeKitap(current);
        current = next;
    }
}

void freeKitapYazarList(KitapYazar *head)
{
    KitapYazar *current = head;
    KitapYazar *next;

    while (current != NULL)
    {
        next = current->next;

        // String'i free et
        free(current->ISBN);

        // Yapıyı free et
        free(current);

        current = next;
    }
}

void freeOduncList(Odunc *head)
{
    Odunc *current = head;
    Odunc *next;

    while (current != NULL)
    {
        next = current->next;

        free(current->ISBN);

        free(current);

        current = next;
    }
}

void freeAllData(Ogrenci *ogrenciHead, Yazar *yazarHead, Kitap *kitapHead,KitapYazar *kitapYazarHead, Odunc *oduncHead)
{

    freeOgrenciList(ogrenciHead);

    freeYazarList(yazarHead);

    freeKitapList(kitapHead);

    freeKitapYazarList(kitapYazarHead);

    freeOduncList(oduncHead);
}
