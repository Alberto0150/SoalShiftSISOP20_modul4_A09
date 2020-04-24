# SoalShiftSISOP20_modul4_A09
#### ADAM ABELARD GARIBALDI 05111840000125
#### ALBERTO SANJAYA 05111840000150
---
Pada bagian ke-1.diminta pertama-tama untuk membuat directory
```
encv1_
```
dimana jika di rename akan menyebabkan directory terenkripsi dimana diketahui key yang digunakan berupa
```
9(ku@AW1[Lmvgax6q`5Y2Ry?+sF!^HKQiBXCUSe&0M.b%rI'7d)o4~VfZ*{#:}ETt$3J-zpc]lnh8,GwP_ND|jO
```

dimana key itu digunakan untuk melakukan enkripsi dan dekripsi. seperti pada contoh yang disediakan:

```
“encv1_rahasia/FOTO_PENTING/kelincilucu.jpg” => “encv1_rahasia/ULlL@u]AlZA(/g7D.|_.Da_a.jpg
```

dengan memanfaatkan caesar chiper.

Kendala yang dialami yaitu belum menemukan algoritma yang tepat sehingga program belum bisa dicompile dan dijalankan

---

Pada bagian ke-2.diminta untuk membuat direktori dengan nama
```
encv2_
```

dimana jika di rename direktori tersebut akan di enkripsi. dimana saat di enkripsi file tersebut akan dibagi-bagi menjadi bagian-bagian sebesar 1024 bytes dan dinamakan menjadi txt dengan tambahan nomor pada belakangnya. 

pada bagian 1 dan 2 baik enkripsi maupun dekripsi akan berjalan hingga kedalam direktori yang berada di dalam directory.

---

Pada bagian ke-3, diminta sinkronisasi pada kedua directory yang di sync kan dengan syarat-syarat seperti :
-Kedua directory memiliki parent directory yang sama.
-Kedua directory kosong atau memiliki isi yang sama. Dua directory dapat dikatakan memiliki isi yang sama jika memenuhi:Nama dari setiap berkas di dalamnya sama dan Modified time dari setiap berkas di dalamnya tidak berselisih lebih dari 0.1 detik.
-Sinkronisasi dilakukan ke seluruh isi dari kedua directory tersebut, tidak hanya di satu child directory saja.
-Sinkronisasi mencakup pembuatan berkas/directory, penghapusan berkas/directory, dan pengubahan berkas/directory.

dimana jika syarat-syarat tersebut tidak terpenuhi maka kedua direktori tidak  tersinkronisasi lagi. dan pada tahap ini tidakdapat menggunakan symbolic links dan thread.

---

Pada bagian ke-4, akan terbentuk file bernama "fs.log" pada direktori "home" pengguna yang berguna untuk menyimpang daftar perintah system call yang dijalankan. dimana pencatatan yang disimpan memiliki format :
```

[LEVEL]::[yy][mm][dd]-[HH]:[MM]:[SS]::[CMD]::[DESC ...]


LEVEL    : Level logging
yy        : Tahun dua digit
mm         : Bulan dua digit
dd         : Hari dua digit
HH         : Jam dua digit
MM         : Menit dua digit
SS         : Detik dua digit
CMD          : System call yang terpanggil
DESC      : Deskripsi tambahan (bisa lebih dari satu, dipisahkan dengan ::)
```

dimana menghasilkan log yang lebih rapi.

LEVEL disini dibagi menjadi INFO dan WARNING, dimana WARNING muncul ketika melakukan syscall rmdir dan unlink saja.

---
