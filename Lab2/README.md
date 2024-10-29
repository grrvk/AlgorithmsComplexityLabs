# Умова

Реалізувати структуру даних фільтр Блума. Елементи-рядки – непусті послідовності довжиною до 15 символів латинських літер. Максимальна кількість елементів у множині не перевищує 10^6. Ймовірність хибнопозитивних спрацювань – 1%.

Структура даних повинна підтримувати операції:
-	перевірки належності рядка множині;
-	додавання рядка до множини (не гарантується, що цього рядка немає у множині).
  
За бажанням можна реалізувати з операцією вилучення (з підрахунком).

__Вхідні дані:__ файл, що містить рядки (<символ операції>  <до 15 символів латинської абетки>).
Загальна кількість рядків у вхідному файлі не перевищує 10^6. Список операцій завершується символом #.

__Вихідні дані:__ для кожної операції перевірки «Y», якщо слово належить множині, «N» - якщо не належить.
