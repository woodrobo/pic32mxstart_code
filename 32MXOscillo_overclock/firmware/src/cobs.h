#ifndef _COBS_H    /* Guard against multiple inclusion */
#define _COBS_H

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

void cobs_decode(uint8_t return_data[], int *return_size, uint8_t packet[], int size){
    *return_size = 0;
    if(size > 256){
        //packet length must be less than 257
        return ;
    }

    int len = size;

    int zero_index = (int)(packet[0] & 0xff);

    int i;
    for(i=1;i<len-1;i++){
        if(i == zero_index){
            return_data[i-1] = 0;
            zero_index = i + (int)(packet[i] & 0xff);
        }else{
            return_data[i-1] = packet[i];
        }
    }
    if((packet[len-1] == 0) && (zero_index == len-1)){
        *return_size = len - 2;
        return;
    }else{
        //zero_index error
        return;
    }
}

/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _COBS_H */
