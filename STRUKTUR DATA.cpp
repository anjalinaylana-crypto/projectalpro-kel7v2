#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <limits>
#include <ctime>
#include <cstdlib>
#include <cctype>
#include <algorithm>

using namespace std;

const int  MAKS_PASIEN    = 100;
const int  JUMLAH_DOKTER  = 5;
const int  MAKS_RIWAYAT   = 10;
const char FILE_RIWAYAT[] = "riwayat_pasien.txt";

string bacaBaris() {
    string s;
    getline(cin, s);
    return s;
}

int bacaInt() {
    int val;
    while (!(cin >> val)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Masukkan angka yang valid: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return val;
}

int bacaIntRange(const string &prompt, int lo, int hi) {
    cout << prompt;
    int val = bacaInt();
    while (val < lo || val > hi) {
        cout << "  Pilihan tidak tersedia! " << prompt;
        val = bacaInt();
    }
    return val;
}

string ambilDigit(const string &s) {
    string hasil = "";
    for (int i = 0; i < (int)s.size(); i++) {
        if (isdigit((unsigned char)s[i]))
            hasil += s[i];
    }
    return hasil;
}

string formatNIK(const string &raw) {
    string d = ambilDigit(raw);
    if (d.size() != 16) return raw;
    return d.substr(0,4) + " " + d.substr(4,4) + " "
         + d.substr(8,4) + " " + d.substr(12,4);
}

string formatTanggal(const string &raw) {
    string d = ambilDigit(raw);
    if (d.size() != 8) return raw;
    string dd = d.substr(0,2);
    string mm = d.substr(2,2);
    string yy = d.substr(4,4);
    int dv = atoi(dd.c_str());
    int mv = atoi(mm.c_str());
    int yv = atoi(yy.c_str());
    if (dv >= 1 && dv <= 31 && mv >= 1 && mv <= 12 && yv >= 1900 && yv <= 2100)
        return dd + "-" + mm + "-" + yy;
    return raw;
}

string formatNoTelp(const string &raw) {
    string d = ambilDigit(raw);
    if (d.empty()) return raw;
    if (d.size() >= 2 && d.substr(0,2) == "62")
        d = d.substr(2);
    if (!d.empty() && d[0] == '0')
        d = d.substr(1);
    if (d.size() < 7 || d.size() > 12) return raw;
    return "+62" + d;
}

string formatRupiah(long nominal) {
    ostringstream oss;
    oss << nominal;
    string s   = oss.str();
    string hasil = "";
    int cnt = 0;
    for (int i = (int)s.size() - 1; i >= 0; i--) {
        if (cnt > 0 && cnt % 3 == 0) hasil = "." + hasil;
        hasil = s[i] + hasil;
        cnt++;
    }
    return "Rp " + hasil;
}

string tanggalHariIni() {
    time_t t = time(NULL);
    tm    *now = localtime(&t);
    ostringstream oss;
    oss << setfill('0')
        << setw(2) << now->tm_mday << "-"
        << setw(2) << (now->tm_mon + 1) << "-"
        << setw(4) << (now->tm_year + 1900);
    return oss.str();
}

void garis(int n = 45) {
    for (int i = 0; i < n; i++) cout << '=';
    cout << "\n";
}

class Dokter {
private:
    string nama;
    string spesialis;
    string namaPerawat;
    string kodeKeluhan;   // token keluhan dipisah spasi, mis. "demam batuk flu"

public:
    Dokter() {}
    Dokter(string nama, string spesialis, string namaPerawat, string kodeKeluhan)
        : nama(nama), spesialis(spesialis), namaPerawat(namaPerawat), kodeKeluhan(kodeKeluhan) {}

    string getNama()        const { return nama; }
    string getSpesialis()   const { return spesialis; }
    string getNamaPerawat() const { return namaPerawat; }

    bool cocokKeluhan(const string &keluhan) const {
        istringstream iss(kodeKeluhan);
        string token;
        while (iss >> token) {
            if (keluhan == token) return true;
        }
        return false;
    }
};

class DaftarDokter {
private:
    Dokter daftar[JUMLAH_DOKTER];

public:
    DaftarDokter() {
        daftar[0] = Dokter("dr. Talitha", "Dokter Umum",              "Siti", "demam batuk flu");
        daftar[1] = Dokter("dr. Andi",    "Spesialis Jantung",        "Rina", "jantung");
        daftar[2] = Dokter("dr. Razya",   "Spesialis Penyakit Dalam", "Dewi", "lambung maag");
        daftar[3] = Dokter("dr. Aya",     "Spesialis Mata",           "Nina", "mata");
        daftar[4] = Dokter("drg. Kiki",   "Dokter Gigi",              "Lia",  "gigi");
    }

    Dokter cariUntukKeluhan(const string &keluhan) const {
        for (int i = 0; i < JUMLAH_DOKTER; i++) {
            if (daftar[i].cocokKeluhan(keluhan)) return daftar[i];
        }
        return daftar[0];
    }

    void tampilkan() const {
        cout << "\n";
        garis();
        cout << "            DAFTAR DOKTER RS\n";
        garis();
        cout << left
             << setw(4)  << "No"
             << setw(16) << "Nama"
             << setw(28) << "Spesialis"
             << "Perawat\n";
        cout << string(60, '-') << "\n";
        for (int i = 0; i < JUMLAH_DOKTER; i++) {
            cout << left
                 << setw(4)  << (i + 1)
                 << setw(16) << daftar[i].getNama()
                 << setw(28) << daftar[i].getSpesialis()
                 << daftar[i].getNamaPerawat() << "\n";
        }
    }
};
