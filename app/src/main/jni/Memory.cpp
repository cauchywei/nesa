//
// Created by Cauchywei on 16/5/14.
//

#include "memory.h"


byte CpuMemory::read(addr_t address) {

    if(address < 0x2000) {
        return ram[address % 0x0800];
    }else if(address < 0x4000){
        //TODO read ppu register
    }else if(address < 0x4014){
        //TODO read APU register
    }else if(address == 0x4014){
        //TODO read PPU register
    }else if(address == 0x4015){
        //TODO read APU register
    }else if(address == 0x4016){
        //TODO read contoller
    }else if(address == 0x4017){
        //TODO read APU register
    }else if(address < 0x6000){
        // TODO: I/O registers
    }else {
        return mapper->read(address);
    }

}



dbyte CpuMemory::readDoubleByte(addr_t address) {
    if(address < 0x2000) {
        return *((dbyte*)(ram + address % 0x0800));
    }else if(address < 0x4000){
        //TODO read ppu register
    }else if(address < 0x4014){
        //TODO read APU register
    }else if(address == 0x4014){
        //TODO read PPU register
    }else if(address == 0x4015){
        //TODO read APU register
    }else if(address == 0x4016){
        //TODO read contoller
    }else if(address == 0x4017){
        //TODO read APU register
    }else if(address < 0x6000){
        // TODO: I/O registers
    }else {
        return mapper->readDoubleByte(address);
    }

    return 0;
}

void CpuMemory::write(addr_t address,byte value){

    if(address < 0x2000) {
        ram[address % 0x0800] = value;
    }else if(address < 0x4000){
        //TODO write ppu register
    }else if(address < 0x4014){
        //TODO write APU register
    }else if(address == 0x4014){
        //TODO write PPU register
    }else if(address == 0x4015){
        //TODO write APU register
    }else if(address == 0x4016){
        //TODO write contoller
    }else if(address == 0x4017){
        //TODO write APU register
    }else if(address < 0x6000){
        // TODO: I/O registers
    }else {
        mapper->write(address,value);
    }
}


void CpuMemory::writeDoubleByte(addr_t address, dbyte value) {
    if(address < 0x2000) {
        *((dbyte*)(ram + address % 0x0800)) = value;
    }else if(address < 0x4000){
        //TODO write ppu register
    }else if(address < 0x4014){
        //TODO write APU register
    }else if(address == 0x4014){
        //TODO write PPU register
    }else if(address == 0x4015){
        //TODO write APU register
    }else if(address == 0x4016){
        //TODO write contoller
    }else if(address == 0x4017){
        //TODO write APU register
    }else if(address < 0x6000){
        // TODO: I/O registers
    }else {
        mapper->writeDoubleByte(address,value);
    }
}
