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
