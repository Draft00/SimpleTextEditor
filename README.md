# SimpleTextEditor
A simple console text editor

Постановка задачи:
Разработать VIM-like текстовый редактор на языке С++. 
Базовые требования:
1.	Программа должна быть написана на языке С++ с использованием STL и/или BOOST под произвольную ОС, выбранную студентом.
2.	Программа должна быть разработана с использованием паттерна MVC.
3.	Программа должна иметь UML диаграмму классов.
4.	Программа должна иметь TUI – Text User Interface. Для разработки TUI разрешается использовать любые open-source библиотеки (например, ncurses). Если библиотеки написаны на языке Си – студент должен разработать класс-обёртку для использования функционала данной библиотеки c использование паттерна Adapter. Требования к Tui:
A.	Разработанный TUI должен быть максимально похож на TUI VIM.
B.	TUI должен поддерживать курсор и строку состояния.
C.	Строка состояния должна отображать:
i.	Текущий режим работы.
ii.	Имя файла.
iii.	Номер строки / Строк всего.
5.	Программа должна работать с 1-байтовой кодировкой текста.
6.	Для работы с текстом, студент должен использовать собственный класс строки в виде статической или динамической библиотеки. Модифицировать исходный код данного класса запрещено. Если какая-либо функциональность не удовлетворяет потребностям студента, студент должен реализовать новый класс с необходимым функционалом на основе существующего c использованием паттерна Decorator.
7.	Программа должна иметь следующие режимы работы: 
A.	Режим навигации и редактирования – основной режим работы, из этого режима осуществляется переход в «Режим ввода» и «Режим поиска». При старте программа должна начинать свою работу именно в этом режиме.
B.	Режим ввода команд. Список поддерживаемых команд представлен в таблице 1:
•	активация режима ввода команд - команда <:>;
•	выход из режима ввода команд в основной режим работы – команда <ESC>.
C.	Режим ввода. Ввод текста на строке с курсором. 
•	активация режима ввода текста перед курсором – с помощью команд из раздела «Ввод текста» таблицы 1;
•	активация режима ввода текста после курсора - команда <o>;
•	очистить текущую строку и начать ввод текста с начала строки - команда S;
•	выход из режима ввода в основной режим работы – команда <ESC>.
D.	Режим поиска. Поиск от курсора до конца или начала документа:
•	активация режима поиска до конца документа - команда </>; 
•	активация режима поиска до начала документа - команда <?>;
•	выход из режима поиска в основной режим работы – команда <ESC>.
