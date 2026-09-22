# Benchmark Prestazioni MicroSD

## Dispositivo e Scheda

* **Modello**: Kingston Canvas Select Plus 64 GB
* **Standard e Classi**: UHS-I, Class 10, U1, V10, A1
* **Velocità Nominale di Lettura**: Fino a 100 MB/s
* **Velocità Minima Garantita di Scrittura**: 10 MB/s (V10 / U1)

---

## Risultati del Test di Scrittura (`dd` su macOS)

* **Data del Test**: 21 Settembre 2026
* **Comando Eseguito**: `dd if=/dev/zero of=/Volumes/bootfs/test_speed.bin bs=1M count=150 conv=sync`
* **Dimensione File di Test**: 157.286.400 byte (150 MB)
* **Tempo Impiegato**: 4,705531 secondi
* **Velocità di Scrittura Reale Misurata**: **33,4 MB/s** (33.425.856 byte/sec)

---

## Analisi Fattibilità per 720p @ 5 FPS

| Formato / Codec Video | Fabbisogno Banda Stimato | Capacità Scheda (33,4 MB/s) | Esito |
| :--- | :--- | :--- | :--- |
| **H.264 / H.265 (Video Compresso)** | 0.5 – 2.0 MB/s | ~15x – 60x superiore al necessario | **Ottimale** |
| **MJPEG / Frame Singoli JPEG** | 1.0 – 3.0 MB/s | ~10x – 30x superiore al necessario | **Ottimale** |
| **RAW YUV420 (Stream Grezzo)** | ~6.9 MB/s | ~4.8x superiore al necessario | **Sufficiente** |
| **RAW RGB24 (Grezzo non compresso)** | ~13.8 MB/s | ~2.4x superiore al necessario | **Sufficiente** |

---

## Conclusioni e Diagnosi

La MicroSD **NON costituisce il collo di bottiglia** per l'implementazione del flusso a **720p @ 5 FPS**. La velocità reale misurata di **33,4 MB/s** supera di gran lunga la soglia minima garantita (10 MB/s) e la banda richiesta dal flusso video.

### Possibili cause di rallentamento se si verificano cali di frame nel daemon

1. **Blocchi I/O Sincroni**: Verificare che le chiamate di scrittura su file non blocchino il thread principale di acquisizione della videocamera (usare buffer in RAM o thread separati).
2. **Overhead di Encoding (CPU)**: Assicurarsi che il tempo per comprimere un singolo frame non superi i **200 ms** ($1 / 5\text{ FPS}$).
3. **Pipeline Sensore / V4L2**: Verificare che l'esposizione automatica in condizioni di scarsa illuminazione non abbassi il framerate hardware nativo della fotocamera.
