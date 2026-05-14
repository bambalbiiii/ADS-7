// Copyright 2025 NNTU-CS
#include "train.h"

Train::Train() : countOp(0), first(nullptr) {}

void Train::addCar(bool light) {
  Car* newCar = new Car;
  newCar->light = light;
  if (!first) {
    first = newCar;
    first->next = first;
    first->prev = first;
  } else {
    Car* last = first->prev;
    last->next = newCar;
    newCar->prev = last;
    newCar->next = first;
    first->prev = newCar;
  }
}

int Train::getOpCount() {
  return countOp;
}

int Train::getLength() {
  if (!first) return 0;
  
  first->light = true;
  Car* current = first;
  int length = 0;

  while (true) {
    length++;
    // Делаем один шаг вперед
    current = current->next;
    countOp++;

    if (current->light) {
      current->light = false;
      // Возвращаемся ровно на length назад
      for (int i = 0; i < length; i++) {
        current = current->prev;
        countOp++;
      }
      // Если маяк погас - нашли длину
      if (!current->light) {
        return length;
      }
      // Если нет - зажигаем маяк снова и продолжаем от него
      current->light = true;
      for (int i = 0; i < length; i++) {
        current = current->next;
        countOp++;
      }
    }
  }
}
