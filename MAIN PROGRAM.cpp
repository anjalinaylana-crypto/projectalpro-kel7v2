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
            case  8: rs.infoDokterMenu();            system("pause"); break;
            case  9: rs.hapusTerakhir();             system("pause"); break;
            case  0: cout << "\nTerima kasih.\n";    break;
        }

    } while (pilihan != 0);

    return 0;
}