#define CSP_RGB24 10
#define CSP_BGR24 15
#define CSP_YUY2 20
#define CSP_I420 30

/* mbco_encoder_init
   инициализирует кодировщик. получает параметры: 
   имя файла с параметрами,
   входящую ширину и высоту, 
   входящий colorspace (CSP_RGB24, CSP_YUY2, CSP_I420),
   требуемую ширину и высоту,
   требуемый битрейт,
   имя дамп файла (для отладки)
   имя лог-файла

   returns 0 on success or -1 for error  
 */
int mbco_encoder_init(char* param_file_name, int width, int height, int csp , int out_width, int out_height, int bitrate, char* dump_file_name, char* log_file_name);



/* mbco_encoder_encode:
   кодирует картинку(фрейм), при этом применяя, если надо масштабирование и переворот,
   и на основе готовит net-пакет.
   на входе получает: буфер с данными, pts, указатель на буфер для результрующего net-пакета,
   указатель на длинну net-пакета.
   NOTES: длинна входящего буфера вычисляется на основе ширины и высоты, указанной при инициализации.

   returns 0 on success or -1 for error
*/
int mbco_encoder_encode(uint8_t *pin, int64_t pts, uint8_t *pout, int *psize);



/* mbco_encoder_headers:
   возвращает net-пакет, содержащий SPS & PPS,
   актуальные для текущего потока 
   
   returns 0 on success or -1 for error
 */
int mbco_encoder_headers(uint8_t *pout, int *psize);


