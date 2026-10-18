# 🔐 RFID Smart Door Lock System with LCD & RGB Status Indicator

Sistem prototipe kunci pintu pintar berbasis mikrokontroler **Arduino Uno** yang memanfaatkan kartu **RFID (Radio Frequency Identification)** sebagai autentikasi akses, motor servo sebagai aktuator fisik kait pintu, layar LCD 16x2 I2C dengan animasi teks berjalan (*non-blocking running text*), serta indikator LED RGB untuk visualisasi status sistem.

---

## 🌟 Fitur Utama

- **Autentikasi Kartu RFID**: Verifikasi Unique Identifier (UID) kartu/gantungan kunci secara instan via modul MFRC522.
- **Kait Pintu Otomatis (Servo)**: Posisi sudut servo terkunci pada $70^\circ$ dan terbuka pada $160^\circ$.
- **Indikator Visual LED RGB**:
  - 🔴 **Merah Solid**: Pintu terkunci / *standby*.
  - 🟢 **Hijau Solid**: Pintu terbuka / akses diterima.
  - 🔵 **Biru Solid**: Proses pembacaan (*scanning*) kartu.
  - 🔴 **Kedip Merah (Alert)**: Akses ditolak / kartu salah.
- **Non-blocking Dual-line Running Text**: Teks pada baris atas dan baris bawah layar LCD bergeser secara halus menggunakan referensi waktu `millis()`, sehingga pembacaan sensor RFID tetap responsif tanpa tertahan fungsi `delay()`.

---

## 🧰 Kebutuhan Komponen

| Komponen | Jumlah | Keterangan |
| :--- | :---: | :--- |
| **Arduino Uno R3** | 1 | Unit mikrokontroler utama |
| **RFID-RC522 Module** | 1 | Pembaca frekuensi 13.56 MHz (SPI) |
| **Servo SG90 / MG90S** | 1 | Aktuator penggerak kait |
| **LCD 16x2 + Modul I2C** | 1 | Penampil teks (Alamat `0x27`) |
| **RGB LED (Common Cathode)** | 1 | Indikator tri-color |
| **Resistor 220 $\Omega$** | 3 | Pembatas arus pin R, G, dan B |
| **Kabel Jumper & Breadboard** | Secukupnya | Rangkaian prototipe |

---

## ⚡ Skema Pengkabelan (Wiring Pinout)

### 1. Modul RFID-RC522 (Komunikasi SPI)
> ⚠️ **Penting**: VCC modul RC522 wajib dihubungkan ke tegangan **3.3V**, bukan 5V!

- **VCC** $\rightarrow$ `3.3V` Arduino
- **RST** $\rightarrow$ `Pin D9`
- **GND** $\rightarrow$ `GND`
- **MISO** $\rightarrow$ `Pin D12`
- **MOSI** $\rightarrow$ `Pin D11`
- **SCK** $\rightarrow$ `Pin D13`
- **SDA / SS** $\rightarrow$ `Pin D10`

### 2. LCD 16x2 (Komunikasi I2C)
- **VCC** $\rightarrow$ `5V`
- **GND** $\rightarrow$ `GND`
- **SDA** $\rightarrow$ `Pin A4`
- **SCL** $\rightarrow$ `Pin A5`

### 3. Motor Servo & Indikator LED RGB
- **Servo PWM (Kabel Oranye)** $\rightarrow$ `Pin D3`
- **Servo VCC (Kabel Merah)** $\rightarrow$ `5V`
- **Servo GND (Kabel Cokelat/Hitam)** $\rightarrow$ `GND`
- **LED Red** $\rightarrow$ Resistor 220 $\Omega$ $\rightarrow$ `Pin D5`
- **LED Green** $\rightarrow$ Resistor 220 $\Omega$ $\rightarrow$ `Pin D6`
- **LED Blue** $\rightarrow$ Resistor 220 $\Omega$ $\rightarrow$ `Pin D9`
- **LED Cathode (Kaki terpanjang)** $\rightarrow$ `GND`

---

## 💻 Panduan Instalasi & Penggunaan

1. **Clone Repositori**:
   ```bash
   git clone https://github.com/<username-kamu>/RFID-Smart-Door.git
   cd RFID-Smart-Door
   ```

2. **Buka Kode Program**:
   Buka file `main.ino` menggunakan **Arduino IDE**.

3. **Install Library yang Dibutuhkan**:
   Buka menu **Tools > Manage Libraries...** di Arduino IDE, lalu pasang:
   - `MFRC522` by GithubCommunity
   - `LiquidCrystal_I2C` by Frank de Brabander / Marco Schwartz

4. **Konfigurasi UID Kartu**:
   Ubah baris kode berikut sesuai dengan UID kartu RFID milikmu:
   ```cpp
   String UID_VALID = "15 75 FE 28"; 
   ```

5. **Upload**:
   Pilih port dan board **Arduino Uno**, kemudian klik tombol **Upload**.

---

## 🌐 Simulasi Daring (Wokwi)

Proyek ini telah dilengkapi dengan berkas `diagram.json`. Kamu dapat membuka dan menjalankan simulasi rangkaian secara langsung melalui platform [Wokwi Arduino Simulator](https://wokwi.com/) tanpa memerlukan hardware fisik.