#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <time.h>
#include <sstream>
#include <iomanip>

// Custom implementation of strptime
tm *strptime(const char *s, const char *f, tm *tm)
{
    std::istringstream input(s);
    input >> std::get_time(tm, f);
    if (input.fail())
    {
        return nullptr;
    }
    return tm;
}

using namespace std;

// Cấu trúc dữ liệu cho Máy bay
struct MayBay
{
    string soHieuMB;
    string loaiMB;
    int soCho;
};

// Cấu trúc dữ liệu cho Vé
struct Ve
{
    int soVe;
    string soCMND;
};

// Cấu trúc dữ liệu cho Chuyến bay
struct ChuyenBay
{
    string maCB;
    tm ngayGioKhoiHanh;
    string sanBayDen;
    int trangThai;
    string soHieuMB;
    list<Ve> danhSachVe;
};

// Cấu trúc dữ liệu cho Hành khách
struct HanhKhach
{
    string soCMND;
    string ho;
    string ten;
    string phai;
};

// Danh sách máy bay
vector<MayBay *> danhSachMayBay;

// Danh sách chuyến bay
list<ChuyenBay> danhSachChuyenBay;

// Cây nhị phân tìm kiếm cho hành khách
map<string, HanhKhach> danhSachHanhKhach;

// Hàm thêm máy bay
void themMayBay(MayBay *mb)
{
    danhSachMayBay.push_back(mb);
}

// Hàm thêm chuyến bay
void themChuyenBay(ChuyenBay cb)
{
    danhSachChuyenBay.push_back(cb);
}

// Hàm thêm hành khách
void themHanhKhach(HanhKhach hk)
{
    danhSachHanhKhach[hk.soCMND] = hk;
}

// Hàm đặt vé
void datVe(string maCB, Ve ve)
{
    for (auto &cb : danhSachChuyenBay)
    {
        if (cb.maCB == maCB)
        {
            cb.danhSachVe.push_back(ve);
            break;
        }
    }
}

// Hàm hủy vé
void huyVe(string maCB, int soVe)
{
    for (auto &cb : danhSachChuyenBay)
    {
        if (cb.maCB == maCB)
        {
            cb.danhSachVe.remove_if([soVe](Ve &ve)
                                    { return ve.soVe == soVe; });
            break;
        }
    }
}

// Hàm in danh sách hành khách thuộc chuyến bay
void inDanhSachHanhKhach(string maCB)
{
    for (const auto &cb : danhSachChuyenBay)
    {
        if (cb.maCB == maCB)
        {
            cout << "=============================================" << endl;
            cout << "DANH SACH HANH KHACH THUOC CHUYEN BAY " << maCB << endl;
            cout << "Ngay gio khoi hanh: " << asctime(&cb.ngayGioKhoiHanh);
            cout << "Noi den: " << cb.sanBayDen << endl;
            cout << "---------------------------------------------" << endl;
            cout << setw(5) << "STT" << setw(10) << "SO VE" << setw(15) << "SO CMND" << setw(20) << "HO TEN" << setw(10) << "PHAI" << endl;
            cout << "---------------------------------------------" << endl;
            int stt = 1;
            for (const auto &ve : cb.danhSachVe)
            {
                HanhKhach hk = danhSachHanhKhach[ve.soCMND];
                cout << setw(5) << stt++ << setw(10) << ve.soVe << setw(15) << ve.soCMND << setw(20) << hk.ho + " " + hk.ten << setw(10) << hk.phai << endl;
            }
            cout << "=============================================" << endl;
            break;
        }
    }
}

// Hàm lưu dữ liệu vào tệp
void luuDuLieu()
{
    ofstream file("data.txt");
    if (file.is_open())
    {
        // Lưu danh sách máy bay
        file << danhSachMayBay.size() << endl;
        for (const auto &mb : danhSachMayBay)
        {
            file << mb->soHieuMB << " " << mb->loaiMB << " " << mb->soCho << endl;
        }

        // Lưu danh sách chuyến bay
        file << danhSachChuyenBay.size() << endl;
        for (const auto &cb : danhSachChuyenBay)
        {
            file << cb.maCB << " " << asctime(&cb.ngayGioKhoiHanh) << " " << cb.sanBayDen << " " << cb.trangThai << " " << cb.soHieuMB << endl;
            file << cb.danhSachVe.size() << endl;
            for (const auto &ve : cb.danhSachVe)
            {
                file << ve.soVe << " " << ve.soCMND << endl;
            }
        }

        // Lưu danh sách hành khách
        file << danhSachHanhKhach.size() << endl;
        for (const auto &hk : danhSachHanhKhach)
        {
            file << hk.second.soCMND << " " << hk.second.ho << " " << hk.second.ten << " " << hk.second.phai << endl;
        }

        file.close();
    }
}

// Hàm đọc dữ liệu từ tệp
void docDuLieu()
{
    ifstream file("data.txt");
    if (file.is_open())
    {
        // Đọc danh sách máy bay
        int soLuongMayBay;
        file >> soLuongMayBay;
        for (int i = 0; i < soLuongMayBay; ++i)
        {
            MayBay *mb = new MayBay;
            file >> mb->soHieuMB >> mb->loaiMB >> mb->soCho;
            danhSachMayBay.push_back(mb);
        }

        // Đọc danh sách chuyến bay
        int soLuongChuyenBay;
        file >> soLuongChuyenBay;
        for (int i = 0; i < soLuongChuyenBay; ++i)
        {
            ChuyenBay cb;
            string ngayGio;
            file >> cb.maCB >> ngayGio >> cb.sanBayDen >> cb.trangThai >> cb.soHieuMB;
            // Custom implementation of strptime
            istringstream ss(ngayGio);
            ss >> get_time(&cb.ngayGioKhoiHanh, "%a %b %d %H:%M:%S %Y");
            strptime(ngayGio.c_str(), "%a %b %d %H:%M:%S %Y", &cb.ngayGioKhoiHanh);
            int soLuongVe;
            file >> soLuongVe;
            for (int j = 0; j < soLuongVe; ++j)
            {
                Ve ve;
                file >> ve.soVe >> ve.soCMND;
                cb.danhSachVe.push_back(ve);
            }
            danhSachChuyenBay.push_back(cb);
        }

        // Đọc danh sách hành khách
        int soLuongHanhKhach;
        file >> soLuongHanhKhach;
        for (int i = 0; i < soLuongHanhKhach; ++i)
        {
            HanhKhach hk;
            file >> hk.soCMND >> hk.ho >> hk.ten >> hk.phai;
            danhSachHanhKhach[hk.soCMND] = hk;
        }

        file.close();
    }
}

// Hàm giao tiếp với người dùng
void menu()
{
    int choice;
    do
    {
        cout << "=============================================" << endl;
        cout << "QUAN LY CHUYEN BAY NOI DIA" << endl;
        cout << "=============================================" << endl;
        cout << "1. Them may bay" << endl;
        cout << "2. Them chuyen bay" << endl;
        cout << "3. Dat ve" << endl;
        cout << "4. Huy ve" << endl;
        cout << "5. In danh sach hanh khach thuoc chuyen bay" << endl;
        cout << "6. Luu du lieu" << endl;
        cout << "7. Doc du lieu" << endl;
        cout << "0. Thoat" << endl;
        cout << "=============================================" << endl;
        cout << "Nhap lua chon: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            MayBay *mb = new MayBay;
            cout << "Nhap so hieu may bay: ";
            cin >> mb->soHieuMB;
            cout << "Nhap loai may bay: ";
            cin >> mb->loaiMB;
            cout << "Nhap so cho: ";
            cin >> mb->soCho;
            themMayBay(mb);
            break;
        }
        case 2:
        {
            ChuyenBay cb;
            cout << "Nhap ma chuyen bay: ";
            cin >> cb.maCB;
            string ngayGio;
            cout << "Nhap ngay gio khoi hanh (dd/mm/yyyy hh:mm): ";
            cin.ignore();
            getline(cin, ngayGio);
            strptime(ngayGio.c_str(), "%d/%m/%Y %H:%M", &cb.ngayGioKhoiHanh);
            cout << "Nhap san bay den: ";
            cin >> cb.sanBayDen;
            cout << "Nhap trang thai (0: huy chuyen, 1: con ve, 2: het ve, 3: hoan tat): ";
            cin >> cb.trangThai;
            cout << "Nhap so hieu may bay: ";
            cin >> cb.soHieuMB;
            themChuyenBay(cb);
            break;
        }
        case 3:
        {
            string maCB, soCMND;
            int soVe;
            cout << "Nhap ma chuyen bay: ";
            cin >> maCB;
            cout << "Nhap so ve: ";
            cin >> soVe;
            cout << "Nhap so CMND: ";
            cin >> soCMND;
            if (danhSachHanhKhach.find(soCMND) == danhSachHanhKhach.end())
            {
                HanhKhach hk;
                hk.soCMND = soCMND;
                cout << "Nhap ho: ";
                cin >> hk.ho;
                cout << "Nhap ten: ";
                cin >> hk.ten;
                cout << "Nhap phai: ";
                cin >> hk.phai;
                themHanhKhach(hk);
            }
            Ve ve{soVe, soCMND};
            datVe(maCB, ve);
            break;
        }
        case 4:
        {
            string maCB;
            int soVe;
            cout << "Nhap ma chuyen bay: ";
            cin >> maCB;
            cout << "Nhap so ve: ";
            cin >> soVe;
            huyVe(maCB, soVe);
            break;
        }
        case 5:
        {
            string maCB;
            cout << "Nhap ma chuyen bay: ";
            cin >> maCB;
            inDanhSachHanhKhach(maCB);
            break;
        }
        case 6:
        {
            luuDuLieu();
            break;
        }
        case 7:
        {
            docDuLieu();
            break;
        }
        case 0:
        {
            cout << "Thoat chuong trinh." << endl;
            break;
        }
        default:
        {
            cout << "Lua chon khong hop le." << endl;
            break;
        }
        }
    } while (choice != 0);
}

int main()
{
    menu();
    return 0;
}