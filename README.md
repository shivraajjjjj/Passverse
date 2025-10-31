# Passverse — Secure Password Manager (C++ / Qt6)

PassVerse is a simple yet powerful **password manager** built with **C++ and Qt6**, designed to store your credentials securely using encryption derived from a master password.  

---

## Features

**Master Password Authentication**  
  Protects all stored credentials using a hashed master password.  
  Master password never stored in plain text.  

**AES Encryption for Credentials**  
  Each credential entry is stored encrypted with a unique IV (Initialization Vector).  
  Data decryption only possible with the correct master password.  

**Add / Update / Delete Credentials**  
 Add new entries easily.  
 Right-click on any entry to **Update** or **Delete** it.  

**Search Functionality**  
 Quickly filter credentials by site name or username.  

**Copy to Clipboard**  
 Instantly copy passwords securely to your clipboard.  

**Session Lock Timeout**  
 Auto-locks the app after a period of inactivity.  

**Responsive Modern UI**  
 Styled with QSS for a clean, dark-themed look.  
 Resizes gracefully on any screen.  

---

## How It Works

  On first launch, PassVerse asks you to **create a master password**.  
  A derived key from this password encrypts all your saved credentials.  
  The hashed master password is saved in a local file (`master.pass`).  
  Credentials are stored in an encrypted SQLite database.  
  If you delete or lose `master.pass`, **data cannot be decrypted** ensuring total security.

---

## Tech Stack

**Language:** C++  
**Framework:** Qt6 (Widgets, SQL, JSON, Core)  
**Database:** SQLite (Encrypted fields)  
**Encryption:** AES (CBC mode with per-entry IVs)  

---

## Installation

1. Download `PassVerse_v1.0.zip`  
2. Extract it to any folder  
3. Run `PassVerse.exe`  
4. Create your master password (first launch only)  
5. Start managing your credentials securely  

> Do **not** delete `master.pass` once created — it’s required to decrypt your credentials.

---

## Developer Notes

To rebuild from source:
```bash
  qmake
  make
Or
open in Qt Creator, then:
  Set build type to Release
  Run windeployqt PassVerse.exe before packaging
