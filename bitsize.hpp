#pragma once

#include <string>
#include <cstring>
#include "rustydef.h"

struct bitsize{
    u64 bytes;
    u8 *data;
    bitsize(const bitsize &bz) : bytes(bz.bytes), data(new u8[bz.bytes]) {
        memcpy(data, bz.data, bytes);
    }
    template<typename...Vargs>
    bitsize(Vargs...varg) : bytes(sizeof...(varg)), data(new u8[sizeof...(varg)]) {
        u8 expansion[] = {static_cast<u8>(varg)...};
        memcpy(data, &expansion, bytes);
    }
    ~bitsize(){
        delete[] data;
    }
    void resize(u64 size){
        delete[] data;
        data = new u8[size];
        memset(data, 0, size);
        bytes = size;
    }
    std::string hex(u64 index){
        std::string h;
        h.resize(2);
        h[0] = '0' + (data[index] >> 4);
        if(h[0] > '9') h[0] += '\7';
        h[1] = '0' + (data[index] & 0xf);
        if(h[1] > '9') h[1] += '\7';
        return h;
    }
    bool getBit(u64 index) const{
        return data[index / 8] & (1 << (index % 8));
    }
    u8 getByte(u64 index) const{
        return data[index];
    }
    void setBit(u64 index, bool val){
        val ? data[index / 8] |= (1 << (index % 8)) : data[index / 8] &= ~(1 << (index % 8));
    }
    void setByte(u64 index, u8 val){
        data[index] = val;
    }
    void trim(){
        u64 index = bytes;
        while(index){
            index--;
            if(getByte(index)) break;
        }
        index++;
        if(index == bytes) return;
        u8 *newData = new u8[index];
        memcpy(newData, data, index);
        delete[] data;
        data = newData;
        bytes = index;
    }
    void operator=(bitsize &&bz){
        delete[] data;
        bytes = bz.bytes;
        data = bz.data;
        bz.data = nullptr;
    }
    void operator=(bitsize &bz){
        delete[] data;
        bytes = bz.bytes;
        data = bz.data;
        bz.data = nullptr;
    }
    bitsize operator+(const bitsize &bz) const{
        short rest = 0;
        bitsize ret(0);
        ret.resize(std::max(bytes, bz.bytes) + 1);
        for(i32 i = 0; i < ret.bytes; ++i){
            if(i < bytes) rest += getByte(i);
            if(i < bz.bytes) rest += bz.getByte(i);
            ret.data[i] = rest & 0xff;
            rest >>= 8;
        }
        ret.trim();
        return ret;
    }
    bitsize operator-(const bitsize &bz) const {
        bitsize ret(0);
        ret.resize(std::max(bytes, bz.bytes) + 1);
        i32 borrow = 0;
        for(i32 i = 0; i < ret.bytes; ++i){
            i32 lhs = (i < bytes) ? getByte(i) : 0;
            i32 rhs = (i < bz.bytes) ? bz.getByte(i) : 0;
            i32 diff = lhs - rhs - borrow;
            if (diff < 0) {
                diff += 256;
                borrow = 1;
            } else {
                borrow = 0;
            }
            ret.setByte(i, diff);
        }
        ret.trim();
        return ret;
    }
    bitsize operator*(const bitsize& bz) const {
        bitsize ret(0);
        ret.resize(bytes + bz.bytes);
        for(i32 i = 0; i < bz.bytes; ++i){
            u8 multiplier = bz.getByte(i);
            u64 carry = 0;
            for (u64 j = 0; j < bytes; ++j) {
                u64 index = i + j;
                u64 product = static_cast<u64>(getByte(j)) * multiplier + ret.getByte(index) + carry;
                ret.setByte(index, product & 0xFF);
                carry = product >> 8;
            }
            if (carry != 0 && (i + bytes) < bytes + bz.bytes) {
                ret.setByte(i + bytes, ret.getByte(i + bytes) + static_cast<u8>(carry));
            }
        }
        ret.trim();
        return ret;
    }
    bool isEmpty(){
        for(i32 i = 0; i < bytes; ++i) if(getByte(i)) return false;
        return true;
    }
};

class pseudo{
    bitsize list;
    u64 index;
public:
    pseudo(bitsize &bz) : list(0), index(0){
        list.bytes = bz.bytes;
        list.data = bz.data;
        bz.bytes = 0;
        bz.data = nullptr;
    }
    template<typename type>
    type get(){
        type ret = 0;
        i(sizeof(type)){
            if(index >= list.bytes) index = 0;
            ret |= static_cast<unsigned long long>(list.getByte(index)) << (i * 8);
            index++;
        }
        return ret;
    }
    void advance(u64 amount){
        index += amount;
    }
    float rangef(){
        return get<unsigned>() / static_cast<float>(-1u);
    }
    float range(){
        return get<unsigned long long>() / static_cast<double>(-1ull);
    }
};

std::ostream &operator<<(std::ostream &o, bitsize bz){
    if(bz.bytes == 0) return o;
    for(u64 s(bz.bytes - 1); s > 0; s--) o << bz.hex(s);
    return o << bz.hex(0);
}

bitsize fact(bitsize x){
    bitsize res(1);
    while(!x.isEmpty()){
        res = res * x;
        x = x - bitsize(1);
    }
    return res;
}

bitsize fibo(bitsize bz){
    if(bz.bytes == 1 && bz.getByte(0) <= 1) return bitsize(1);
    bitsize aux1(1), aux2(1), res(0);
    do{
        res = aux1 + aux2;
        aux1 = aux2;
        aux2 = res;
        bz = bz - bitsize(1);
    } while(bz.bytes > 1 || bz.getByte(0) > 2);
    return aux2;
}