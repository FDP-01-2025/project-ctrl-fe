#pragma once
//* Configraciones básicas de la consola
#include "./settings/console.h"
#include "./settings/maping.h"

void identfierMap(int key = 1){
    switch(key){

        case 1:
         //Cargar el mapa principal
            putMap(key);
        break;
        default:
        break;
    }
}