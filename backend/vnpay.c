#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include <time.h>
#include <sys/socket.h>
#include <ctype.h>
#include <arpa/inet.h>

#define VNP_SECRET_KEY "35G7ZC5AYLSPMTEGB116107LGA5GN2E2" 

char *url_encode(const char *str) {
    char *encoded = malloc(strlen(str) * 3 + 1);
    char *pstr = encoded;
    for (size_t i = 0; str[i]; i++) {
        if (isalnum((unsigned char)str[i])) {
            *pstr++ = str[i];
        } else {
            sprintf(pstr, "%%%02X", (unsigned char)str[i]);
            pstr += 3;
        }
    }
    *pstr = '\0';
    return encoded;
}

char *hmac_sha512(const char *key, const char *data) {
    unsigned char *result;
    unsigned int len = SHA512_DIGEST_LENGTH;
    result = HMAC(EVP_sha512(), key, strlen(key), (unsigned char*)data, strlen(data), NULL, &len);
    
    char *hex_result = malloc(len * 2 + 1);
    for (int i = 0; i < len; i++) {
        sprintf(hex_result + i * 2, "%02x", result[i]);
    }
    hex_result[len * 2] = '\0';
    return hex_result;
}

int main() {
    // Các tham số cần thiết
    char *vnp_Version = "2.1.0";
    char *vnp_Command = "pay";
    char *vnp_TmnCode = "Q7WL2NKX";  // Thay bằng mã website của bạn
    int vnp_Amount = 1806000 * 100;  // Số tiền thanh toán (ví dụ: 1806000 VND => 180600000)
    char *vnp_CurrCode = "VND";
    char *vnp_IpAddr = "127.0.0.1";  // Địa chỉ IP của khách hàng
    char *vnp_Locale = "vn";  // Ngôn ngữ giao diện (vn hoặc en)
    char *vnp_OrderInfo = "haha";  // Thông tin đơn hàng
    char *vnp_OrderType = "other";  // Loại đơn hàng
    char *vnp_ReturnUrl = "https://d64b-2001-ee0-4001-1c50-d622-bb2a-ebee-e96.ngrok-free.app/ReturnUrl";  // URL trả về sau khi thanh toán
    char *vnp_TxnRef = "5";  // Mã giao dịch duy nhất
    char *vnp_CreateDate = "20241221172333";  // Thời gian giao dịch
    char *vnp_ExpireDate = "20241221173533";  // Thời gian hết hạn
    char *vnp_SecureHash;

    // Xây dựng chuỗi tham số (query string) theo thứ tự bảng chữ cái
    char query_string[1024];
    snprintf(query_string, sizeof(query_string), 
        "vnp_Version=%s&vnp_Command=%s&vnp_TmnCode=%s&vnp_Amount=%d&vnp_CurrCode=%s&vnp_IpAddr=%s&vnp_Locale=%s&vnp_OrderInfo=%s&vnp_OrderType=%s&vnp_ReturnUrl=%s&vnp_TxnRef=%s&vnp_CreateDate=%s&vnp_ExpireDate=%s",
        vnp_Version, vnp_Command, vnp_TmnCode, vnp_Amount, vnp_CurrCode, vnp_IpAddr, vnp_Locale, vnp_OrderInfo, vnp_OrderType, vnp_ReturnUrl, vnp_TxnRef, vnp_CreateDate, vnp_ExpireDate);

    // Mã hóa URL cho query_string
    char *encoded_query = url_encode(query_string);

    // Tính toán HMAC SHA512 để tạo vnp_SecureHash
    vnp_SecureHash = hmac_sha512(VNP_SECRET_KEY, encoded_query);

    // Tạo URL thanh toán
    printf("Payment URL: https://sandbox.vnpayment.vn/paymentv2/vpcpay.html?%s&vnp_SecureHash=%s\n", query_string, vnp_SecureHash);

    // Giải phóng bộ nhớ
    free(encoded_query);
    free(vnp_SecureHash);
    return 0;
}
