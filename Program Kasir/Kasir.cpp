#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

struct Barang {
    string nama;
    int harga;
    int stok;
};

string formatRibuan(int angka) {
    string s = to_string(angka);
    int n = s.length();
    string result = "";
    int count = 0;
    for (int i = n - 1; i >= 0; i--) {
        result = s[i] + result;
        count++;
        if (count % 3 == 0 && i != 0) result = "." + result;
    }
    return result;
}

char inputYesNo(string pesan) {
    char pilihan;
    while (true) {
        cout << pesan;
        cin >> pilihan;
        pilihan = tolower(pilihan);
        if (pilihan == 'y' || pilihan == 'n') return pilihan;
        cout << "Input salah, masukkan y/n.\n";
    }
}

int inputUang(string pesan) {
    string bayarStr;
    int bayar;
    while (true) {
        cout << pesan;
        cin >> bayarStr;
        string bayarBersih = "";
        for(char c : bayarStr) if(c != '.') bayarBersih += c;
        stringstream ss(bayarBersih);
        if(ss >> bayar) return bayar;
        cout << "Input tidak valid, masukkan angka.\n";
    }
}

int main() {
    vector<Barang> data = {
        {"Beras",12000,0},
        {"Gula",14000,0},
        {"Minyak",16000,0},
        {"Telur",22000,0},
        {"Kopi",8000,0},
        {"Teh",7000,0},
        {"Susu",15000,0},
        {"Sabun",5000,0},
        {"Mi",3000,0},
        {"Roti",6000,0}
    };

    int totalPendapatanHariIni = 0;
    int totalPendapatanKeseluruhan = 0;
    int pelanggan = 0;

    ifstream fileStok("stok.txt");
    if(fileStok.is_open()) {
        string nama;
        int jumlah;
        for(int i=0;i<data.size();i++) {
            fileStok >> nama >> jumlah;
            data[i].stok = jumlah;
        }
        fileStok.close();
    } else {
        int defaultStok[10] = {100,40,30,35,60,55,25,45,100,30};
        for(int i=0;i<data.size();i++) data[i].stok = defaultStok[i];
    }

    ifstream filePendapatan("pendapatan.txt");
    if(filePendapatan.is_open()) {
        filePendapatan >> totalPendapatanKeseluruhan;
        filePendapatan.close();
    }

    char tambahStok = inputYesNo("Ingin menambah stok barang sebelum toko dibuka? (y/n): ");
    while(tambahStok == 'y') {
        int nomor, jumlah;
        cout << "Pilih nomor barang (1-" << data.size() << "): ";
        cin >> nomor;
        cout << "Jumlah tambahan: ";
        cin >> jumlah;
        if(nomor >= 1 && nomor <= data.size()) {
            data[nomor-1].stok += jumlah;
            cout << "Stok baru " << data[nomor-1].nama << ": " << data[nomor-1].stok << endl;
        }
        tambahStok = inputYesNo("Tambah stok barang lain? (y/n): ");
    }

    char tambahBarang = inputYesNo("Ingin menambah barang baru? (y/n): ");
    while(tambahBarang == 'y') {
        Barang baru;
        cout << "Nama barang baru: ";
        cin.ignore();
        getline(cin, baru.nama);
        cout << "Harga: ";
        cin >> baru.harga;
        cout << "Stok awal: ";
        cin >> baru.stok;
        data.push_back(baru);
        cout << "Barang " << baru.nama << " berhasil ditambahkan.\n";
        tambahBarang = inputYesNo("Tambah barang baru lagi? (y/n): ");
    }

    char lanjutPelanggan = 'y';
    cout << "====================\nTOKO DIBUKA\n====================\n";

    while(lanjutPelanggan == 'y') {
        pelanggan++;
        int totalBelanja = 0;
        char lanjutBarang = 'y';

        while(lanjutBarang == 'y') {
            cout << "\nDaftar Barang:\n";
            for(int i=0;i<data.size();i++) {
                if(data[i].stok > 0)
                    cout << i+1 << ". " << data[i].nama
                         << " Rp" << formatRibuan(data[i].harga)
                         << " Stok: " << data[i].stok << endl;
            }

            int pilih, jumlah;
            cout << "Pilih barang: ";
            cin >> pilih;
            cout << "Jumlah beli: ";
            cin >> jumlah;

            if(pilih >= 1 && pilih <= data.size() && data[pilih-1].stok >= jumlah) {
                totalBelanja += data[pilih-1].harga * jumlah;
                data[pilih-1].stok -= jumlah;
            }

            lanjutBarang = inputYesNo("Tambah barang? (y/n): ");
        }

        cout << "\nTotal belanja: Rp" << formatRibuan(totalBelanja) << endl;
        int bayar = inputUang("Uang bayar: Rp");
        cout << "Kembalian: Rp" << formatRibuan(bayar - totalBelanja) << endl;

        totalPendapatanHariIni += totalBelanja;
        lanjutPelanggan = inputYesNo("\nMasih ada pelanggan? (y/n): ");
    }

    cout << "\n====================\nTOKO TUTUP\n====================\n";
    cout << "Jumlah pelanggan hari ini: " << pelanggan << endl;
    cout << "Total pendapatan hari ini: Rp" << formatRibuan(totalPendapatanHariIni) << endl;

    ofstream fileOut("stok.txt");
    for(int i=0;i<data.size();i++)
        fileOut << data[i].nama << " " << data[i].stok << endl;
    fileOut.close();

    totalPendapatanKeseluruhan += totalPendapatanHariIni;
    ofstream filePend("pendapatan.txt");
    filePend << totalPendapatanKeseluruhan;
    filePend.close();

    cout << "Tekan Enter untuk keluar...";
    cin.ignore();
    cin.get();

    return 0;
}
