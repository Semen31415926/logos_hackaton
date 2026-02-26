// Пины
const int BUTTON_PIN = 2;   // Пин кнопки (подтяжка к питанию)
const int RELAY_PIN = 3;    // Пин управления реле

// Переменные состояния
int lastButtonState = HIGH;  // Предыдущее состояние кнопки (HIGH - не нажата, т.к. PULLUP)
unsigned long lastDebounceTime = 0;  // Время последнего изменения состояния
unsigned long debounceDelay = 50;    // Задержка антидребезга (50 мс)

void setup() {
  // Настройка пинов
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Включаем внутреннюю подтяжку к 5В
  pinMode(RELAY_PIN, OUTPUT);
  
  // Начальное состояние - реле выключено
  // Важно! У разных модулей реле логика может быть инвертирована.
  // Обычно: LOW - выключено, HIGH - включено.
  digitalWrite(RELAY_PIN, LOW);
  
  // Для отладки (можно открыть монитор порта)
  // Serial.begin(9600);
}

void loop() {
  // Читаем текущее состояние кнопки (LOW - нажата, т.к. замыкает на GND)
  int reading = digitalRead(BUTTON_PIN);

  // Если состояние изменилось (из-за шума или нажатия), сбрасываем таймер
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  // Проверяем, прошло ли достаточно времени с последнего изменения
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Если прошло больше debounceDelay, значит состояние стабильно
    if (reading != lastButtonState) {
      // Состояние действительно изменилось
      lastButtonState = reading;

      // Реагируем на нажатие (LOW - кнопка замкнута на GND)
      if (lastButtonState == LOW) {
        // Кнопка НАЖАТА - включаем реле (греем проволоку)
        digitalWrite(RELAY_PIN, HIGH);
        
        // Для отладки
        // Serial.println("RELAY ON - Heating");
        
        // Если нужно, чтобы грелась только пока кнопка нажата - оставляем как есть.
        // Если нужно включать/выключать по одному нажатию (триггер) - нужно усложнять код.
      } else {
        // Кнопка ОТПУЩЕНА - выключаем реле
        digitalWrite(RELAY_PIN, LOW);
        
        // Для отладки
        // Serial.println("RELAY OFF");
      }
    }
  }

  // Сохраняем текущее состояние для следующего цикла
  // (lastButtonState уже обновлен выше, но здесь мы ничего не делаем)
}
