//
//  main.cpp
//  hook
//
//  Created by Виткина Анна on 18.04.2023.
//
#include <iostream>
#include <ApplicationServices/ApplicationServices.h>

static CGEventRef eventCallback(CGEventTapProxy proxy, CGEventType type, CGEventRef event, void *userInfo)
{
    if (type == kCGEventKeyDown) {
        // Получаем код нажатой клавиши
        CGKeyCode keyCode = (CGKeyCode)CGEventGetIntegerValueField(event, kCGKeyboardEventKeycode);
        
        // Получаем модификаторы (Shift, Ctrl, Option, Command)
        CGEventFlags flags = CGEventGetFlags(event);
        
        // Выводим информацию о нажатой клавише в терминал
        std::cout << "keyCode: " << keyCode << ", flags: " << flags << std::endl;
    }
    
    // Пропускаем все события, кроме нажатия клавиши
    return event;
}

int main(int argc, const char * argv[])
{
    // Создаем событие для перехвата клавиатуры
    CFMachPortRef eventTap = CGEventTapCreate(kCGSessionEventTap,
                                              kCGHeadInsertEventTap,
                                              kCGEventTapOptionDefault,
                                              kCGEventMaskForAllEvents,
                                              eventCallback,
                                              nullptr);
    
    // Создаем цикл обработки событий
    CFRunLoopSourceRef runLoopSource = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, eventTap, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CGEventTapEnable(eventTap, true);
    
    // Запускаем цикл обработки событий
    CFRunLoopRun();
    
    // Очищаем ресурсы
    CFRunLoopRemoveSource(CFRunLoopGetCurrent(), runLoopSource, kCFRunLoopCommonModes);
    CFRelease(runLoopSource);
    CFMachPortInvalidate(eventTap);
    CFRelease(eventTap);
    
    return 0;
}

