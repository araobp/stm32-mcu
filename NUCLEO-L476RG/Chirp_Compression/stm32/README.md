# Chirp compression

## Paramters

|Parameter    |Value |
|------------|------|
|System clock|80MHz|
|PDM clock|80M/25=3.2MHz|
|DFSDM data rate|80M/25/42=76.2kHz|
|DFSDM LPF|sinc3|
|DFSDM data resolution|16bit|
|Buffer|2048 bytes|
|Chirp signal|1024 bytes + 1024 bytes zero padding|
|Matched filter|1024 bytes + 1024 bytes zero padding|
|Chirp sweep range|16000-19000Hz|
|Chirp frame length|13.4 msec|

