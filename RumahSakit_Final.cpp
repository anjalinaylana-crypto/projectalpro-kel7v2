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

class RiwayatKunjungan {
private:
    string tanggal, keluhan, dokter, status;

public:
    RiwayatKunjungan() {}
    RiwayatKunjungan(string tanggal, string keluhan, string dokter, string status)
        : tanggal(tanggal), keluhan(keluhan), dokter(dokter), status(status) {}

    void tampilkanBaris() const {
        cout << left << setw(12) << tanggal
             << setw(15) << keluhan
             << setw(18) << dokter
             << status << "\n";
    }
};

class RiwayatPasien {
private:
    RiwayatKunjungan data[MAKS_RIWAYAT];
    int jumlah;

public:
    RiwayatPasien() : jumlah(0) {}

    bool tambah(const RiwayatKunjungan &r) {
        if (jumlah >= MAKS_RIWAYAT) return false;
        data[jumlah] = r;
        jumlah++;
        return true;
    }

    void tampilkan() const {
        cout << "\n--- Riwayat Kunjungan (Sesi Ini) ---\n";
        if (jumlah == 0) {
            cout << "  Belum ada riwayat.\n";
            return;
        }
        cout << left << setw(12) << "Tanggal"
             << setw(15) << "Keluhan"
             << setw(18) << "Dokter"
             << "Status\n";
        cout << string(58, '-') << "\n";
        for (int k = 0; k < jumlah; k++) data[k].tampilkanBaris();
    }
};

class RiwayatFile {
public:
    static void simpan(const string &nik, const string &nama, const string &keluhan,
                        const string &dokter, const string &status, long biaya) {
        ofstream f;
        f.open(FILE_RIWAYAT, ios::app);
        if (!f.is_open()) return;
        f << "---\n"
          << "NIK="     << nik     << "\n"
          << "NAMA="    << nama    << "\n"
          << "TGL="     << tanggalHariIni() << "\n"
          << "KELUHAN=" << keluhan << "\n"
          << "DOKTER="  << dokter  << "\n"
          << "STATUS="  << status  << "\n"
          << "BIAYA="   << biaya   << "\n";
        f.close();
    }

    static void tampilkanUntukNIK(const string &nik) {
        ifstream f;
        f.open(FILE_RIWAYAT);
        if (!f.is_open()) {
            cout << "  (File riwayat belum tersedia)\n";
            return;
        }

        string line;
        string nikFile, tgl, keluhan, dokter, status;
        bool dalamBlok = false;
        bool adaData   = false;

        cout << "\n--- Riwayat dari File ---\n";
        cout << left << setw(12) << "Tanggal"
             << setw(15) << "Keluhan"
             << setw(18) << "Dokter"
             << "Status\n";
        cout << string(58, '-') << "\n";

        while (getline(f, line)) {
            if (line == "---") {
                if (dalamBlok && nikFile == nik) {
                    cout << left << setw(12) << tgl
                         << setw(15) << keluhan
                         << setw(18) << dokter
                         << status << "\n";
                    adaData = true;
                }
                dalamBlok = true;
                nikFile = tgl = keluhan = dokter = status = "";
                continue;
            }
            int pos = (int)line.find('=');
            if (pos == (int)string::npos) continue;
            string key = line.substr(0, pos);
            string val = line.substr(pos + 1);
            if      (key == "NIK")     nikFile  = val;
            else if (key == "TGL")     tgl      = val;
            else if (key == "KELUHAN") keluhan  = val;
            else if (key == "DOKTER")  dokter   = val;
            else if (key == "STATUS")  status   = val;
        }
        if (dalamBlok && nikFile == nik) {
            cout << left << setw(12) << tgl
                 << setw(15) << keluhan
                 << setw(18) << dokter
                 << status << "\n";
            adaData = true;
        }
        f.close();
        if (!adaData)
            cout << "  Belum ada riwayat tersimpan untuk NIK ini.\n";
    }
};

class Pasien {
private:
    string nama, nik, tglLahir, jk, alamat, noTelp;
    string keluhan, riwayatPenyakit, alergi;
    string rsTujuan, alasanRujukan;

    Dokter dokter;   // komposisi: dokter yang menangani pasien ini

    string statusPasien, kategoriPerawatan;
    long   biayaPerHari, totalBiaya, diskonBPJS, diskonInap;
    int    pilihanStatus, lamaInap, pilihanPembayaran, pilihanKamar;

    RiwayatPasien riwayat;

    long hitungBiayaRekursif(int hari, long biaya) const {
        if (hari == 0) return 0;
        return biaya + hitungBiayaRekursif(hari - 1, biaya);
    }

public:
    Pasien() {
        biayaPerHari = totalBiaya = diskonBPJS = diskonInap = 0;
        lamaInap = pilihanKamar = pilihanPembayaran = pilihanStatus = 0;
        kategoriPerawatan = "-";
    }

    void inputData() {
        cout << "\n";
        garis();
        cout << "   SISTEM PENERIMAAN PASIEN RUMAH SAKIT\n";
        garis();
        cout << "\n";

        cout << left << setw(28) << "Nama Pasien"          << ": "; nama            = bacaBaris();
        cout << left << setw(28) << "NIK/KTP (16 digit)"   << ": "; nik             = formatNIK(bacaBaris());
        cout << left << setw(28) << "Tgl Lahir (DDMMYYYY)" << ": "; tglLahir        = formatTanggal(bacaBaris());
        cout << left << setw(28) << "Jenis Kelamin (L/P)"  << ": "; jk              = bacaBaris();
        cout << left << setw(28) << "Alamat"               << ": "; alamat          = bacaBaris();
        cout << left << setw(28) << "No. HP"               << ": "; noTelp          = formatNoTelp(bacaBaris());
        cout << left << setw(28) << "Keluhan"              << ": "; keluhan         = bacaBaris();
        cout << left << setw(28) << "Riwayat Penyakit"     << ": "; riwayatPenyakit = bacaBaris();
        cout << left << setw(28) << "Alergi"               << ": "; alergi          = bacaBaris();

        cout << "\n--- Status Pasien ---\n";
        cout << "1. Rawat Inap\n";
        cout << "2. Rawat Jalan\n";
        cout << "3. Rujukan ke RS Lain\n\n";
        pilihanStatus = bacaIntRange("Pilihan : ", 1, 3);

        if (pilihanStatus == 1) {
            cout << "\n--- Pilihan Kamar ---\n";
            cout << "1. Bangsal  (" << formatRupiah(200000) << "/hari)\n";
            cout << "2. VIP      (" << formatRupiah(500000) << "/hari)\n\n";
            pilihanKamar = bacaIntRange("Pilihan : ", 1, 2);

            cout << "\nLama Inap (hari) : ";
            lamaInap = bacaInt();
            while (lamaInap <= 0) {
                cout << "  Harus lebih dari 0!\nLama Inap : ";
                lamaInap = bacaInt();
            }

            cout << "\n--- Pilihan Pembayaran ---\n";
            cout << "1. BPJS (diskon 20%)\n";
            cout << "2. Umum\n\n";
            pilihanPembayaran = bacaIntRange("Pilihan : ", 1, 2);

        } else if (pilihanStatus == 2) {
            cout << "\n--- Pilihan Pembayaran ---\n";
            cout << "1. BPJS (diskon 20%)\n";
            cout << "2. Umum\n\n";
            pilihanPembayaran = bacaIntRange("Pilihan : ", 1, 2);

        } else {
            cout << "\n--- Data Rujukan ---\n";
            cout << "Rumah Sakit Tujuan : "; rsTujuan      = bacaBaris();
            cout << "Alasan Rujukan     : "; alasanRujukan = bacaBaris();
        }
    }

    void tetapkanDokter(const DaftarDokter &daftarDokter) {
        dokter = daftarDokter.cariUntukKeluhan(keluhan);
    }

    void hitungBiaya() {
        diskonInap = 0;
        diskonBPJS = 0;

        if (pilihanStatus == 1) {
            statusPasien  = "Rawat Inap";
            biayaPerHari  = (pilihanKamar == 1) ? 200000L : 500000L;
            totalBiaya    = hitungBiayaRekursif(lamaInap, biayaPerHari);

            if (lamaInap > 5) {
                diskonInap  = totalBiaya * 10 / 100;
                totalBiaya -= diskonInap;
            }
            if (pilihanPembayaran == 1) {
                diskonBPJS  = totalBiaya * 20 / 100;
                totalBiaya -= diskonBPJS;
            }

            if      (lamaInap <= 3) kategoriPerawatan = "Ringan";
            else if (lamaInap <= 7) kategoriPerawatan = "Sedang";
            else                    kategoriPerawatan = "Intensif";

        } else if (pilihanStatus == 2) {
            statusPasien      = "Rawat Jalan";
            biayaPerHari      = 0;
            totalBiaya        = 50000L;
            kategoriPerawatan = "-";

            if (pilihanPembayaran == 1) {
                diskonBPJS  = totalBiaya * 20 / 100;
                totalBiaya -= diskonBPJS;
            }
        } else {
            statusPasien      = "Rujukan";
            biayaPerHari      = 0;
            totalBiaya        = 0;
            kategoriPerawatan = "-";
        }
    }

    void catatKunjungan() {
        RiwayatKunjungan r(tanggalHariIni(), keluhan, dokter.getNama(), statusPasien);
        riwayat.tambah(r);
        RiwayatFile::simpan(nik, nama, keluhan, dokter.getNama(), statusPasien, totalBiaya);
    }

    void tampilkan(int nomor) const {
        garis();
        cout << "  DATA PASIEN  (No. Antrian " << nomor << ")\n";
        garis();
        cout << left << setw(24) << "Nama"          << ": " << nama     << "\n";
        cout << left << setw(24) << "NIK"           << ": " << nik      << "\n";
        cout << left << setw(24) << "Tanggal Lahir" << ": " << tglLahir << "\n";
        cout << left << setw(24) << "Jenis Kelamin" << ": " << jk       << "\n";
        cout << left << setw(24) << "Alamat"        << ": " << alamat   << "\n";
        cout << left << setw(24) << "No. Telepon"   << ": " << noTelp   << "\n";

        cout << "\n--- Data Medis ---\n";
        cout << left << setw(24) << "Keluhan"          << ": " << keluhan         << "\n";
        cout << left << setw(24) << "Riwayat Penyakit" << ": " << riwayatPenyakit << "\n";
        cout << left << setw(24) << "Alergi"           << ": " << alergi          << "\n";

        cout << "\n--- Status Pasien ---\n";
        cout << left << setw(24) << "Status" << ": " << statusPasien << "\n";
        if (statusPasien == "Rawat Inap") {
            string namaKamar = (pilihanKamar == 1) ? "Bangsal" : "VIP";
            cout << left << setw(24) << "Kamar"     << ": " << namaKamar << "\n";
            cout << left << setw(24) << "Lama Inap" << ": " << lamaInap  << " Hari\n";
        }
        if (statusPasien == "Rujukan") {
            cout << left << setw(24) << "RS Tujuan"      << ": " << rsTujuan      << "\n";
            cout << left << setw(24) << "Alasan Rujukan" << ": " << alasanRujukan << "\n";
        }
        cout << left << setw(24) << "Kategori" << ": " << kategoriPerawatan << "\n";

        cout << "\n--- Tenaga Medis ---\n";
        cout << left << setw(24) << "Dokter"    << ": " << dokter.getNama()        << "\n";
        cout << left << setw(24) << "Spesialis" << ": " << dokter.getSpesialis()   << "\n";
        cout << left << setw(24) << "Perawat"   << ": " << dokter.getNamaPerawat() << "\n";

        riwayat.tampilkan();
        RiwayatFile::tampilkanUntukNIK(nik);

        cout << "\n--- Pembayaran ---\n";
        if (statusPasien == "Rawat Inap") {
            cout << left << setw(24) << "Biaya/Hari"  << ": " << formatRupiah(biayaPerHari) << "\n";
            cout << left << setw(24) << "Diskon Inap" << ": " << formatRupiah(diskonInap)   << "\n";
            cout << left << setw(24) << "Diskon BPJS" << ": " << formatRupiah(diskonBPJS)   << "\n";
        } else if (statusPasien == "Rawat Jalan") {
            cout << left << setw(24) << "Diskon BPJS" << ": " << formatRupiah(diskonBPJS)   << "\n";
        }
        cout << left << setw(24) << "Total Biaya" << ": " << formatRupiah(totalBiaya) << "\n";
        cout << "\n";
    }

    void tampilkanRingkas(int nomor) const {
        cout << left
             << setw(4)  << nomor
             << setw(22) << nama
             << setw(22) << nik
             << setw(14) << statusPasien
             << formatRupiah(totalBiaya) << "\n";
    }

    string getNama() const { return nama; }
};

class RumahSakit {
private:
    DaftarDokter daftarDokter;
    Pasien       daftarPasien[MAKS_PASIEN];
    int          jumlahPasien;

public:
    RumahSakit() : jumlahPasien(0) {}

    bool antrianPenuh()  const { return jumlahPasien >= MAKS_PASIEN; }
    bool antrianKosong() const { return jumlahPasien == 0; }

    void tambahPasienBaru() {
        if (antrianPenuh()) {
            cout << "\nAntrian penuh! (maks " << MAKS_PASIEN << " pasien)\n";
            return;
        }

        Pasien p;
        p.inputData();
        p.tetapkanDokter(daftarDokter);
        p.hitungBiaya();
        p.catatKunjungan();

        daftarPasien[jumlahPasien] = p;
        jumlahPasien++;

        cout << "\nPasien berhasil ditambahkan. Total antrian: " << jumlahPasien << "\n";
        system("pause");
    }

    void tampilAntrianAwalKeAkhir() {
        if (antrianKosong()) { cout << "\nAntrian kosong.\n"; return; }
        cout << "\n";
        garis();
        cout << "   ANTRIAN PASIEN  (AWAL -> AKHIR)\n";
        garis();
        Pasien *ptr = daftarPasien;        // pointer ke elemen pertama
        for (int i = 0; i < jumlahPasien; i++, ptr++)
            ptr->tampilkan(i + 1);
    }

    void tampilAntrianAkhirKeAwal() {
        if (antrianKosong()) { cout << "\nAntrian kosong.\n"; return; }
        cout << "\n";
        garis();
        cout << "   ANTRIAN PASIEN  (AKHIR -> AWAL)\n";
        garis();
        Pasien *ptr = daftarPasien + (jumlahPasien - 1);  // pointer ke elemen terakhir
        for (int i = jumlahPasien - 1; i >= 0; i--, ptr--)
            ptr->tampilkan(i + 1);
    }

    void sortAZ() {
        if (antrianKosong()) { cout << "\nAntrian kosong.\n"; return; }
        for (int i = 0; i < jumlahPasien - 1; i++) {
            int idx = i;
            for (int j = i + 1; j < jumlahPasien; j++)
                if (daftarPasien[j].getNama() < daftarPasien[idx].getNama()) idx = j;
            if (idx != i) swap(daftarPasien[i], daftarPasien[idx]);
        }
        cout << "\nData diurutkan A-Z berdasarkan nama.\n";
    }

    void sortZA() {
        if (antrianKosong()) { cout << "\nAntrian kosong.\n"; return; }
        for (int i = 0; i < jumlahPasien - 1; i++) {
            int idx = i;
            for (int j = i + 1; j < jumlahPasien; j++)
                if (daftarPasien[j].getNama() > daftarPasien[idx].getNama()) idx = j;
            if (idx != i) swap(daftarPasien[i], daftarPasien[idx]);
        }
        cout << "\nData diurutkan Z-A berdasarkan nama.\n\n";
        daftarRingkas();
    }

    void cariNama() {
        if (antrianKosong()) { cout << "\nAntrian kosong.\n"; return; }
        cout << "\nMasukkan nama pasien : ";
        string kata = bacaBaris();
        bool ketemu = false;
        for (int i = 0; i < jumlahPasien; i++) {
            if (daftarPasien[i].getNama() == kata) {
                daftarPasien[i].tampilkan(i + 1);
                ketemu = true;
            }
        }
        if (!ketemu) cout << "\nPasien \"" << kata << "\" tidak ditemukan.\n";
    }

    void daftarRingkas() const {
        if (antrianKosong()) { cout << "\nAntrian kosong.\n"; return; }
        cout << "\n";
        garis();
        cout << "         DAFTAR RINGKAS PASIEN\n";
        garis();
        cout << left
             << setw(4)  << "No"
             << setw(22) << "Nama"
             << setw(22) << "NIK"
             << setw(14) << "Status"
             << "Total Biaya\n";
        cout << string(75, '-') << "\n";

        const Pasien *ptr = daftarPasien;
        for (int i = 0; i < jumlahPasien; i++, ptr++)
            ptr->tampilkanRingkas(i + 1);

        cout << "\nTotal pasien dalam antrian: " << jumlahPasien << "\n";
    }

    void infoDokterMenu() const {
        daftarDokter.tampilkan();
    }

    void hapusTerakhir() {
        if (antrianKosong()) { cout << "\nAntrian kosong.\n"; return; }
        jumlahPasien--;
        cout << "\nPasien terakhir dihapus. Sisa antrian: " << jumlahPasien << "\n";
    }
};

void tampilMenu() {
    cout << "\n";
    for (int i = 0; i < 45; i++) cout << '=';
    cout << "\n";
    cout << "    SISTEM ADMINISTRASI RUMAH SAKIT\n";
    for (int i = 0; i < 45; i++) cout << '=';
    cout << "\n";
    cout << "  1.  Tambah Pasien Baru\n";
    cout << "  2.  Tampilkan Antrian (Awal -> Akhir)\n";
    cout << "  3.  Tampilkan Antrian (Akhir -> Awal)\n";
    cout << "  4.  Urutkan Nama (A-Z)\n";
    cout << "  5.  Urutkan Nama (Z-A)\n";
    cout << "  6.  Cari Pasien (Nama)\n";
    cout << "  7.  Info Dokter\n";
    cout << "  8.  Hapus Pasien Terakhir\n";
    cout << "  0.  Keluar\n";
    for (int i = 0; i < 45; i++) cout << '=';
    cout << "\n";
    cout << "Pilihan : ";
}

int main() {
    RumahSakit rs;
    int pilihan;

    do {
        tampilMenu();
        pilihan = bacaInt();
        while (pilihan < 0 || pilihan > 8) {
            cout << "  Pilihan tidak tersedia (0-8): ";
            pilihan = bacaInt();
        }

        switch (pilihan) {
            case  1: rs.tambahPasienBaru();          break;
            case  2: rs.tampilAntrianAwalKeAkhir(); system("pause"); break;
            case  3: rs.tampilAntrianAkhirKeAwal(); system("pause"); break;
            case  4: rs.sortAZ();                   system("pause"); break;
            case  5: rs.sortZA();                   system("pause"); break;
            case  6: rs.cariNama();                 system("pause"); break;
            case  7: rs.infoDokterMenu();            system("pause"); break;
            case  8: rs.hapusTerakhir();             system("pause"); break;
            case  0: cout << "\nTerima kasih.\n";    break;
        }

    } while (pilihan != 0);

    return 0;
}
