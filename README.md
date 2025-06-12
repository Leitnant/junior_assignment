# Drooni ja GCS suhtulse simulaator
Loodud KrattWorks *Junior developer assignment*-iks.

 - Language:			C++20
 - Build System:		CMake

## Kompileerimine ja käivitamine
Minu kasutatud kompilaator on **MSVC++**, kuid **G++** peaks ka teoorias töötama.

Jooksuta kõigepealt **GCS.exe**, ning seejärel **Drone.exe**.
Droon on *by-default* **disarmed**, kordinaatidel `X: 0 Y:0 ALT: 0` ja *geofence* on `200x200x200` kuup keskmega nullkoordinaatidel.
Windows 10-ga testides ei olnud tegelikult vahet kumb enne alustada.

## Juhtimine
GCS aknasse, kuhu peaks ka kirjutama sisendi, jookseb drooni telemeetria, mille tõttu kaob sisendi *prompt* kiiresti silmist. 
**Saadaval on kolm käsku:**
 - ARM - sisesta GCS aknasse `1`
	 - droon hakkab liikuma määratud sihtpunkti.
 - DISARM - sisesta GCS aknasse `2`
	 - droon jääb seisma.
 - GOTO - sisesta GCS aknasse `3`
	 - siis tuleb sisestada X koordinaat. Nt: `30`
	 - siis tuleb sisestada Y koordinaat. Nt: `20`
	 - siis tuleb sisestada ALT koordinaat. Nt: `100`

## Telemeetria
**[HBT]:**  kuvan möödunud aega viimasest saabunud *heartbeat*-ist `CONNECTED` / `DISCONNECTED` asemel
**[ARM]:** `GUIDED_ARMED`/`GUIDED_DISARMED`
**[ X ]:** `X koordinaat`
**[ Y ]:** `Y koordinaat`
**[ALT]:** `ALT koordinaat`
