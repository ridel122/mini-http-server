# Mini HTTP Server

## Deskripsi

Mini HTTP Server berbasis bahasa C yang berjalan pada sistem Linux/Unix.

Server ini mengimplementasikan protokol HTTP/1.1 secara sederhana dan mampu:

* Menerima koneksi dari browser (Chrome, Firefox, dll)
* Memproses request HTTP GET
* Mengirim file statis HTML, CSS, dan gambar
* Menangani banyak klien secara bersamaan menggunakan `fork()`
* Menangani zombie process menggunakan `SIGCHLD`
* Mengembalikan status HTTP 200, 400, 404, dan 405

---

## Struktur Proyek

```text
project/
│
├── main.c
├── core_network.c
├── core_network.h
├── http_parser.c
├── http_parser.h
├── response_builder.c
├── response_builder.h
├── Makefile
├── README.md
│
└── www/
    ├── index.html
    ├── style.css
    ├── logo.png
    ├── 400.html
    ├── 404.html
    └── 405.html
```

---

## Pembagian Tugas

### Bagian 1 – Core Network & Concurrent Engineer

Implementasi:

* socket()
* setsockopt()
* bind()
* listen()
* accept()
* fork()
* SIGCHLD
* waitpid()

File:

```text
core_network.c
core_network.h
```

---

### Bagian 2 – HTTP Parser & Protocol Specialist

Implementasi:

* recv()
* Parsing HTTP Request
* Validasi Method GET
* Error 400 Bad Request
* Error 405 Method Not Allowed

File:

```text
http_parser.c
http_parser.h
```

---

### Bagian 3 – File I/O & Response Builder

Implementasi:

* stat()
* fopen()
* fread()
* MIME Type
* Error 404 Not Found
* HTTP Response Header
* send()

File:

```text
response_builder.c
response_builder.h
```

---

## Kompilasi

Masuk ke direktori proyek:

```bash
cd project
```

Compile:

```bash
make
```

---

## Menjalankan Server

Contoh pada port 8080:

```bash
./server 8080
```

Output:

```text
Mini HTTP Server Running
Port : 8080
```

---

## Pengujian

### 1. HTTP 200 OK

Buka browser:

```text
http://localhost:8080
```

Server akan mengirim file:

```text
www/index.html
```

---

### 2. HTTP 404 Not Found

Buka:

```text
http://localhost:8080/tidak_ada.html
```

Server akan menampilkan:

```text
404 Not Found
```

---

### 3. HTTP 405 Method Not Allowed

Gunakan curl:

```bash
curl -X POST http://localhost:8080
```

Server akan mengembalikan:

```text
405 Method Not Allowed
```

---

### 4. HTTP 400 Bad Request

Gunakan netcat:

```bash
nc localhost 8080
```

Ketik:

```text
HELLO SERVER
```

Server akan mengembalikan:

```text
400 Bad Request
```

---

## Browser yang Didukung

* Google Chrome
* Mozilla Firefox
* Microsoft Edge
* Browser lain yang mendukung HTTP/1.1

---

## Author

Nama: Angelina Sembel,
      Galileo Pelealu,
      Riedel Hamel

Mata Kuliah: Unix Programming

Tahun: 2026
