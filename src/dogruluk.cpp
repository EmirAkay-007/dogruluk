
//    PROJE İSMİ : Doğruluk Veri Yapısı 
// 
//
//    TEMEL PRENSİP:
//
//   - Durak düğümleri sadece durağın Numarasını tutar.
//
//   - Zaman bilgisi düğümlerde değil,
//     iki durak arasındaki geçişte tutulur.
//
//   - Her geçiş, önceki geçişlerin süresinide dikkate alır.
//
//   - Kullanıcıya yalnızca " Gelmesine kalan dakika " bilgisi gösterilir.



#include <iostream> // "cin" ve "cout" gibi işlemleri yapabilmek için
#include <locale.h> // Türkçe karakterler kullanabilmek için.

using namespace std;


// GEÇİŞ VERİSİ ( veri yolda hesaplanır )

struct GecisVerisi {
    double anlikSure;        // Hesaplama için kullanılır.
    double kumulatifSure;    // Gelmesine kalan toplam süre.
    int saat;                // saat verisini tutar.
    int binemeyen;           // binemeyen sayısını tutar.
};

// DURAK DÜĞÜMÜ ( sadece konum )

struct DurakDugumu {
    int durakNO;             //   Durak numarasını tutar          
    GecisVerisi* ileriGecis; //   hesaplama verisini işaret eder 
    DurakDugumu* sonraki;    //  sonraki düğümü işaret eder       

    //                    sade bir yapısı var
    //   çünkü Durak düğümü sadece kimlik ve adres bilgisi taşıyor.
    // Zaman, gecikme veya hesaplanmış herhangi bir değer burada değil
    //                  geçiş verisinin içinde.


    DurakDugumu(int no) : durakNO(no), ileriGecis(nullptr), sonraki(nullptr) {}
};



// Ana veri yapısı
class Dogruluk {
private:

    // Önemli olduklarından dolayı dışardan müdahaleye 
    //    kapatmak için "private" içinde tanımladım

     // sürekli bir döngü ile listenin sonuna gitmemek için
    DurakDugumu* bas;      // veri yapısının başlangıcı
    DurakDugumu* son;      // veri yapısının sonu
    GecisVerisi* sonGecis; // Önceki geçişi tutar

    int saat;
    int binemeyen;


    // Saat katsayısı
    double saatKatsayisi(int s) {
        if (s >= 7 && s <= 9)  return 1.3;
        if (s >= 12 && s <= 14) return 1.2;
        if (s >= 17 && s <= 19) return 1.4;
        return 1.0;
    }


    // Yolcu yoğunluğu katsayısı 
    double kapasiteKatsayisi(int k) {
        if (k == 0) return 1.0;
        if (k <= 5) return 1.4;
        if (k <= 10) return 1.5;
        return 1.6;
    }

    // Geliş süresi hesaplama
    double anlikSureHesapla() {
        double sure = 1;
        return sure * saatKatsayisi(saat) * kapasiteKatsayisi(binemeyen) * 2;
    }


public:
    Dogruluk(int s, int b) : bas(nullptr), son(nullptr),
        sonGecis(nullptr),
        saat(s), binemeyen(b) {
    }



    // DURAK EKLEME
    void durakEkle(int no) {
        DurakDugumu* yeniDurak = new DurakDugumu(no);
        // yeni durak oluşturma

        if (bas == nullptr) { //Sona gelmişse
            bas = son = yeniDurak;
        }
        else {  // Sona gelmemişse
            GecisVerisi* gv = new GecisVerisi;

            gv->anlikSure = anlikSureHesapla();
            gv->saat = saat;
            gv->binemeyen = binemeyen;


            // birikimli hesaplama  ( gelmesine kalan dakika )

            if (sonGecis == nullptr) // sona gelmişse
                gv->kumulatifSure = gv->anlikSure;
            // kümilatif değeri son değere eşitler

            else      // Sona gelmemişse
                gv->kumulatifSure =
                sonGecis->kumulatifSure + gv->anlikSure;

            son->ileriGecis = gv;
            son->sonraki = yeniDurak;

            sonGecis = gv;
            son = yeniDurak;
        }
    }

    //YAZDIRMA
    void yazdir() {
        DurakDugumu* temp = bas; // ilk değerden başlayarak

        cout << "\n--- DURAK BAZLI GELMESİNE KALAN SÜRE ---\n\n";

        // sonuna kadar ilerler
        while (temp != nullptr && temp->sonraki != nullptr) {
            GecisVerisi* gv = temp->ileriGecis;

            cout << "Durak " << temp->durakNO << " -> " <<
                temp->sonraki->durakNO << " | Gelmesine Kalan Dakika: " <<
                gv->kumulatifSure << " dk\n";

            temp = temp->sonraki;
        }
    }

    //BELLEK SİLME
    void temizle() {
        while (bas != nullptr) {
            if (bas->ileriGecis != nullptr)
                delete bas->ileriGecis;

            DurakDugumu* temp = bas;
            bas = bas->sonraki;      // tüm yapıyı sırayla dolaşır
            delete temp;
        }

        son = nullptr;         // son kalan değerleride sıfıra eşitler
        sonGecis = nullptr;
    }
};

// ANA fonksiyon
int main() {
    setlocale(LC_ALL, "Turkish");

    int durakSayisi, saat, binemeyen;

    cout << "\n\n\n";
    cout << "Durak sayısı: ";
    cin >> durakSayisi;
    cout << "\n";

    for (;;) {
        cout << "Saat (6-23): ";
        cin >> saat;
        cout << "\n";

        if (saat >= 6 && saat <= 23)
            break;   // geçerli değer alındığında döngüden çıkar
    }

    cout << "Durakta bekleyen yolcu sayısı: ";
    cin >> binemeyen;
    cout << "\n";

    Dogruluk sistem(saat, binemeyen);

    for (int i = 1; i <= durakSayisi; i++) {
        sistem.durakEkle(i);
    }
    sistem.yazdir();     // değerleri yazdırır
    sistem.temizle();    // belleğin içini boşaltır.

    return 0;
}