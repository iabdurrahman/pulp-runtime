#include <Print.h>

size_t Print::write(byte value){

}

size_t Print::write(const char *string){

}

size_t Print::write(const uint8_t *data, size_t length){

}

// Parameters val allowed any data type. The missing implemented data types are bool, boolean, byte, short, word, float and string().

size_t Print::print(char val){
    return write(val);  // Print as it is
}

size_t Print::print(const char *val){
    return write(val);  // Print as it is
}

size_t Print::print(unsigned char val){
    return print((unsigned long) val);
}

size_t Print::print(int val){
    return print((long) val);
}

size_t Print::print(unsigned int val){
    return print((unsigned long) val);
}

size_t Print::print(long val){
    
}

size_t Print::print(unsigned long val){
    
}

size_t Print::print(const __FlashStringHelper *val){
    
}

size_t Print::print(unsigned char val, int format){
    return print((unsigned long) val, format);
}

size_t Print::print(int val, int format){
    return print((long) val, format);
}

size_t Print::print(unsigned int val, int format){
    return print((unsigned long) val, format);
}

size_t Print::print(long val, int format){
    if (format == BIN){

    }
    else if(format == OCT){

    }
    else if(format == DEC){

    }
    else if(format == HEX){

    }
}

size_t Print::print(unsigned long val, int format){
    if (format == BIN){

    }
    else if(format == OCT){

    }
    else if(format == DEC){

    }
    else if(format == HEX){

    }
}

size_t Print::print(double val, int format){
    // Format parameter specifies the number of decimal places to use
}

size_t Print::println(unsigned char val){
    return println((unsigned long) val);
}

size_t Print::println(int val){
    return println((long) val);
}

size_t Print::println(unsigned int val){
    return println((unsigned long) val);
}

size_t Print::println(long val){
    
}

size_t Print::println(unsigned long val){
    
}

size_t Print::println(double val){
    
}

size_t Print::println(unsigned char val, int format){
    return println((unsigned long) val, format);
}

size_t Print::println(int val, int format){
    return println((long) val, format);
}

size_t Print::println(unsigned int val, int format){
    return println((unsigned long) val, format);
}

size_t Print::println(long val, int format){
    if (format == BIN){

    }
    else if(format == OCT){

    }
    else if(format == DEC){

    }
    else if(format == HEX){

    }
}

size_t Print::println(unsigned long val, int format){
    if (format == BIN){

    }
    else if(format == OCT){

    }
    else if(format == DEC){

    }
    else if(format == HEX){

    }
}

size_t Print::println(double val, int format){
    // Format parameter specifies the number of decimal places to use
}

size_t Print::println(const __FlashStringHelper *val){
    
}
