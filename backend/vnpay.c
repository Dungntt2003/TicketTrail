#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/hmac.h>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include <time.h>
#include <ctype.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define VNP_SECRET_KEY "7637DTK8WAH42HZALTO0CMHU5DFBWE24"  // Thay bằng key của bạn

// Hàm mã hóa URL (URL encoding)
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

// Hàm tính toán HMAC SHA512
char *hmac_sha512(const char *key, const char *data) {
    unsigned char *result;
    unsigned int len = SHA512_DIGEST_LENGTH;
    result = HMAC(EVP_sha512(), key, strlen(key), (unsigned char*)data, strlen(data), NULL, &len);
    
    // Chuyển sang chuỗi hex
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
    char *vnp_TmnCode = "Q7WL2NKX";
    int vnp_Amount = 1806000 * 100;  // Số tiền thanh toán, nhân 100
    char *vnp_CurrCode = "VND";
    char *vnp_IpAddr = "127.0.0.1";  // Địa chỉ IP của khách hàng
    char *vnp_Locale = "vn";
    char *vnp_OrderInfo = "Thanh toan don hang :5";
    char *vnp_OrderType = "other";
    char *vnp_ReturnUrl = "https://domainmerchant.vn/ReturnUrl";
    char *vnp_TxnRef = "5";  // Mã giao dịch duy nhất
    char *vnp_CreateDate = "20210801153333";  // Thời gian giao dịch
    char *vnp_ExpireDate = "20210801183333";  // Thời gian hết hạn
    char *vnp_SecureHash;

    // Xây dựng chuỗi query (thứ tự alphabet)
    char query_string[1024];
    snprintf(query_string, sizeof(query_string), 
        "vnp_Version=%s&vnp_Command=%s&vnp_TmnCode=%s&vnp_Amount=%d&vnp_CurrCode=%s&vnp_IpAddr=%s&vnp_Locale=%s&vnp_OrderInfo=%s&vnp_OrderType=%s&vnp_ReturnUrl=%s&vnp_TxnRef=%s&vnp_CreateDate=%s&vnp_ExpireDate=%s",
        vnp_Version, vnp_Command, vnp_TmnCode, vnp_Amount, vnp_CurrCode, vnp_IpAddr, vnp_Locale, vnp_OrderInfo, vnp_OrderType, vnp_ReturnUrl, vnp_TxnRef, vnp_CreateDate, vnp_ExpireDate);

    // Mã hóa URL
    char *encoded_query = url_encode(query_string);

    // Tính toán HMAC SHA512
    vnp_SecureHash = hmac_sha512(VNP_SECRET_KEY, encoded_query);

    // In URL thanh toán VNPAY
    printf("Payment URL: https://sandbox.vnpayment.vn/paymentv2/vpcpay.html?%s&vnp_SecureHash=%s\n", query_string, vnp_SecureHash);

    free(encoded_query);
    free(vnp_SecureHash);
    return 0;
}
