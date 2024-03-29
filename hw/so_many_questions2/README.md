# So many questions 2
by Kaloyan Doychinov and Krasimir Trenchev

## Файлови системи, overview

1. Как се казва директорията прародител на всички директории?

    В UNIX системите главната директория, директорията на всички директории, т.нар прародител е root директорията. Тъй като файловата система е йерархична и се представлява от дърво/граф. То затова root директорията е корена на това дърво/граф - началото на всичко. Във файловата система се отбелязва със символа ```/```.


2. Споменахме 2 начина за рефериране на обекти във файловата система (пътища), кои са те? С какво се характеризират? Как ОС "разбира" (оценява) кой път до кой файл води?

    Има два вида пътища във файловата система до файлове / директории: 

    - абсолютен път (absolute path) - път до файлове / директории, който започва от root директорията. Започва със символа ```/```.

        ```bash
        $ pwd
        /home/kaloyan/documents
        $ cd /home/kaloyan/downloads
        $ pwd
        /home/kaloyan/downloads
        ```

    - релативен път (relative path) - път до файлове / директории, който започва от текущата директория. Започва със символа ```./```.

        ```bash
        $ pwd
        /home/kaloyan
        $ cd downloads
        $ pwd
        /home/kaloyan/downloads
        ```
    
        *обаче релативният път не започва само от текущата директория (но зависи от нея), примерно може да започнем от по-горната директория на текущата с ```..``` (parent / upper directory)*, докато от текущата директория се обозначава с ```.``` (current directory).

3. Колко типа файлове съществуват в GNU/Linux операционните системи, кои са те?

    В GNU/Linux всичко (букв всичко) се репрезентира чрез файлове, затова не е толкова просто да ги наричаме папки и файлове (както в windows). Тук идва и красотата на Linux. Имаме цели 8 вида файлове, които се различават по типа си. Те биват:

    - *Regular файлове [-]* – файлове, които съдържат информация в текстов или бинарен формат. Винаги, когато създадем файл, се маркира ***inode*** с ***link count*** (равен на 0), с който е класифициран този файл. Терминът ***Payload*** се използва за съдържанието, което е записано в самите файловете
    - *Директории [d]* – файлове (0_0). В data block-овете на тези файлове се записва информация за това кои са обектите и inode-ве в тази директория (примерно изглежда така: ```\<path>:\<inode>``` за всеки файл в тази директория)
    - *Hard link [?]* – предоставят ново име на вече резервиран inode (т.е. идеята е да се предостави ново име на вече съществуващ файл); нов път, който се асоциира с inode, който вече е зает

        Създава се с ```ln``` командата. Сам по себе си hard link-а не хаби (заделя) inode

        *Не може да се направи hard link към файл извън нашата файлова система, защото всяка файлова система си има отделна inode таблица. Също не може да има hard link към несъществуващ файл*

    - *Symlink [l]* – или още познат като link или shortcut; това са файлове, които реферират ("сочат") към други файлове, представляват път към друг файл или директория във файловата система

        Symlink-овете заемат inode. И за разлика от hard link-овете може да обхващат различни файлови системи. В data block-а пише: ```\<path>```. За да се създаде се използва командата ```ln -s```

        *Един проблем, който идва с това е, че може да се получат т.нар broken (sym)link - когато symlink-а сочи към файл, който не съществува.*

    - *Named pipes [p]* – или само pipes – реферират буферирана памет в memory space-a на kernel-а. Named pipes се използват за комуникация между процеси. Това е обект, зад който стои pipe.
    - *Sockets [s]* – pipes за интернет. Позволява два процеса, работещи в две различни ОС, да си прехвърлят информация през интернет
    - *Character special [c]* – Репрезентират хардуерни устройства или софтуер, който се прави на хардуерни устройства. Char special репрезентират устройства, които обработват информация байт по байт (мишка напр.)
    - *Block special [b]* – Block репрезентират устройства, който обработват информация блок по блок (хард дискове напр.).

4. Какви права за достъп съществуват? Какво ни позволява всяко от правата да правим с даден файл? (hint: file vs directory)

    В GNU/Linux операционните системи, правата за достъп се използват за да се определи кой има право да чете, пише или изпълнява даден файл или директория. Правата за достъп се управляват чрез файловата система и се изразяват чрез три основни символа:
    - r (Read) - означава право за четене на файловете, а при директориите - право за преглед на съдържанието на директорията.
    - w (Write) - означава право за запис на файловете, а при директориите - право за създаване, преименуване или премахване на файлове и под директории.
    - x (eXecute) - означава право за изпълнение на файловете, а при директориите - право за влизане в директорията.

5. Съществуват 2 типа репрезентации на права за достъп, кои са те, какъв е техния формат?

    В GNU/Linux операционната система, правата за достъп се представят като ***10-битови числа***, където първият символ определя типа на обекта (файл или директория) и останалите девет символа са разделени на три групи по три символа за правата за достъп за съответно потребител, група и останали потребители

    Тези символи могат да бъдат зададени и от страна на потребителя, използвайки командата chmod. Например, правата за достъп на файл "test.txt" могат да бъдат зададени като ```rw-r--r--```, което означава, че потребителят има права за четене и писане, а групата и останалите потребители имат право само за четене на файла

    Има два общоприети начина за представяне на правата за достъп:
    - Числов:
        - Бинарен (Двоичен) - 111 110 000 (rwx rw- —)
        - Осмичен - 760 (rwx rw- —)
    - Символен
        - rwx rw- — (111 110 000)

6. Каква информация се пази в `/etc/passwd` файла? Какъв е неговият формат?

    В този файл се съдържа информация за потребителите на системата - т.е. лист с потребителите, като преди да се хешират паролите, те са се съхранявали в този файл. Всеки един ред представлява информация за един потребител.

    А този ред е разделен на колони чрез ```:``` Колоните биват:
    - Username - потребителското име
    - Password - както беше споменато по-горе, тази колона вече не се използва, тъй като this practice is deemed insecure. Затова паролите вече се пазят във ```/etc/shadow```, където са достъпни само за root потребителя, но и не само това, те са и хеширани.
    - UID - User ID
    - GID - Group ID - ID за текущата активна група на потребителя
    - User Information - допълнителна информация за потребителя - примерно (ако не се лъжа) в Ubuntu се използва за пълното имен на потребителя и т.н.
    - Home directory - пътят до home директорията на потребителя
    - Login shell - пътят до shell-a, който се използва от потребителя - примерно /bin/bash - т.е. първата команда, която се изпълнява при вписване в shell-a

    Ето и пример за това как изглежда извадка от /etc/passwd файла на macOS:

    ```
    nobody:*:-2:-2:Unprivileged User:/var/empty:/usr/bin/false
    root:*:0:0:System Administrator:/var/root:/bin/sh
    daemon:*:1:1:System Services:/var/root:/usr/bin/false
    ```

7. За какво се използва `/home` директорията?

    При създаването на нови потребители, по подразбиране тяхната home директория се създава в `/home`, като името на директорията е същото като потребителското име (в повечето случаи)

    Повечето потребители имат home директории. Тези директории се използват като енкапсулирано място, където потребителя контролира своите директории - все едно това е стаята на потребителя. 

## Файлови системи, имплементация

8. Разделихме файловите системи на две части - Мета информация и Data Block Table. Каква информация се пази в мета информацията? От какво се състои Data Block Table-a?

    Делим байтовете във файловата система на мета информация и data block table(равна по големина поредица от блокове).
    - Мета информация:
        - Типът на файловата система (zfs, ext4, ext3, apfs, ntfs, exfat, ...)
        - Големината на data block-овете – основна конфигурационна черта
        - Масив от inodes
            - Inode – абстракцията на структурата, с която се имплементира файла във файловата система. В тях се пази всичко за файла освен пътя до този inode. Пазят се 2 типа информация: мета информация за файла и съдържание.
        - Мета информацията:
            - owner user + owner group
            - permisions
            - size
            - atime – момента в който за последно файла е бил достъпван
            - mtime – момента в който за последно съдържанието на този файл е било редактирано 
            - ctime – момента в който за последно мета информацията е била редактирана за последно
            - link count
    - Data block table - самото съдържание на файловете в една файлова система се пази в тук. В тази таблица (от 13 елемента записваме самите данни на файла) Тези елементи са малки фрагменти от памет с определена големина, които са с еднакъв размер (по-късно ще разгледаме)

9. Какво е inode?

    Inode е абстракцията на структурата, с която се имплементира файла във файловата система. В нея се пази всичко за файла освен пътя до този inode. Пазят се 2 типа информация: мета информация за файла и съдържание. Всеки inode има специален уникален идентификационен номер

10. Какви полета има в inode-ите? Какъв е техният смисъл?

    Полетата, които се съдържат в inode-ите са:
    - owner user + owner group
    - permisions
    - size
    - atime – момента в който за последно файла е бил достъпван
    - mtime – момента в който за последно съдържанието на този файл е било редактирано 
    - ctime – момента в който за последно мета информацията е била редактирана за последно
    - link count - броя на линкове към файла
    - препратки към data block table-a (2 реда по-надолу)

    Един inode може да съдържа до 13 полета за препратки към data block table-a. Тези полета са:
        - 1-10 - в първите 10 полета се записват номера на data block-ове от data block table-a. Първоначално се пише в data block-a сочен от първото  поле. Ако файлът е по-голям от това, се използва следващото поле - намира свободен data block, към който полето да сочи, и в него се пише информацията.
        - 11 - single indirect data block (едномерен масив) - ако съдържанието на файла не може да се съхрани в 10-те data block-a, сочени от първите 10 полета се използва 11-тото (аналогично е и за следващите). 11-тото поле се използва за съхраняване на указател към data block, който съдържа указатели към допълнителни data block-ове, където се съхраняват части от данните на файла
        - 12 - double indirect data block (двумерен масив) - съдържа указател към data block, който съдържа указатели към други data block-ове, които от своя страна също съдържат указатели към data block-ове, където се съхраняват части от данните на файла (масив от 11-ти елемент)
        - 13 - triple indirect data block (тримерен масив) - съдържа указател към data block, който също съдържа указатели към други data block-ове, които също съдържат указатели към data block-ове, където се съхраняват части от данните на файла. (масив от 12-ти елемент)

11. Какъв е форматът на UNIX time?

    Началото на света се определя от UNIX Time - 01.01.1970г. Оттогава всяка секунда е станала част от UNIX. Форматът бива следния: `1683829699` - 32-битово число, което представлява броя секунди, изминали от началото на UNIX Time.

12. Какво зависи от големината на Data Block-овете?

    От големината на data block-овете зависи колко ефективно (от гледна точка на бързина и на място за съхранение) е запазването на инфомацията на външното запаметяващо устройство. Ако дата блоковете са прекалено големи, голяма част от тях ще останат празни, което ще доведе до излишна загуба на пространсво (прим: 4KB file в 1МB block device ще заема физическо пространсво от 1МB, а не 4KB). Ако дата блоковете са прекалено малки, ще се налага да се използват много от тях, което ще забави достъпа до информацията, забавяне при четене и писане. Но и не само това - тъй като inode таблицата е лимитирана по размер, то тя може да сочи само определен брой data block-ове, което ограничава максималния размер на файловете.

## Регулярни изрази

13. Какви типове (категории) символи със специално значени изучихме? Кои са символите във всяка категория? Какъв е техния смисъл?

    За regex сме изучавали 2 типа символи:
    - Без специално значение: символите от азбуката, цифри и т.н
    - Със специално значение:
        - Anchors:
            - `^` - начало на ред
            - `$` -> край на ред
        - Classes:
            - `[abc]` - един от символите в класа (в този случай - "a" или "b", или "c") трябва да присъства
            - `[^a]` - който и да е символ, различен от описаните в класa (в този случай - "а")
        - Groups - `(<exp>)` - всичко между скобите е група
        - Квантори - символи, които оказват повторение на атома (всяка буква от азбуката, група или клас), след който за записани, пишат се след атоми. Има 4 квантора:
            - `+` - едно или повече повторения на атома
            - `*` - нула или повече повторения
            - `?` - нула или едно повторения
            - `{<n>}` - точно n повторения след атома
            - `{<n>,<m>}` - от n до m повторения след атома
        - Екраниращ символ (Escape Symbol) - `\` - ако пред специален символ, напишем екраниращ символ, специалният символ спира да бъде специален, т.е. става без специално значение, обикновен символ (например: `\.` match-ва точка, а не всички символи)
        - Wildcard - `.` (match-ва рандом символ от азбуката)

        За повече информация, както казахте, може да видим ```man 7 regex``` или [тук](https://www.gnu.org/software/grep/manual/html_node/Regular-Expressions.html)

14. Дайте поне 3 примера за валидни регулярни изрази и обяснете какво match-ват.
    - `^([a-zA-Z0-9]+)@([a-zA-Z0-9]+)\.([a-zA-Z0-9]+)$` - match-ва валиден email адрес (много basic - съществуват много, МНОГО по-добри изрази)
    - `^(\+359|0)(87|88|89|98)[0-9]{7}$` - match-ва валиден български телефонен номер (в повечето случаи)
    - `^([0-9]{2})\.([0-9]{2})\.([0-9]{4})$` - match-ва валидна дата във формат dd.mm.yyyy
    - `.*[02468]` - match-ва всички четни числа (този е пример от часа, остналите съм ги ползвал в някои стари сайтове (без този за имейла, че той малко (без коментар за него))

## Команди

14. С коя команда можем да изведем информация за:

    14.1. Процесите в рамките на операционната ни система?

    ```bash
    ps 
    ```

    14.2. Свободното пространство в рамките на mount-натите в момента файлови системи?

    ```bash
    df
    ```

    14.3. I/O операциите извършвани от процесите?

    ```bash
    iotop
    ```

    14.4. CPU употребата по процеси?

    ```bash
    top
    ```

    14.5. Употребата на оперативна памет?

    ```bash
    free
    ```

    14.6. Потребителите с активни логин сесии?

    ```bash
    who
    ```

15. Избройте 5-те ви любими команди, които сме учили. Опишете (с 1-2 изречения) за какво се използва всяка от тях. Дайте пример.

    ```bash
    top # показва информация за процесите в системата, ама реално предпочитам htop
    ```

    ```bash
    sudo # позволява изпълнение на команди с права на root, но има една по-готина, която не е въобще не е комплексна и мощна, но върши същата работа, за която ние ползваме sudo. И тя е doas :)
    ```

    ```bash
    find # търси файлове, директории по дадени критерии (флагове), честно казано най-добрата (и спасяваща команда)
    ```

    ```bash
    grep # търси във файлове или във входа на командата (stdin) за съвпадения на регулярен израз
    ```

    ```bash
    man # показва ръководство за дадена команда (или командата, която ни спасява по ОС - библията на Linux)
    ```

## Shell

16. За следните redirection-и - '>', '|', '<<<', '<':

    16.1. Какви операнди приемат?

    - \> - отляво е команда (но не е задължително), както и номер на файлов дескриптор (по подразбиране е 1 - stdout), отдясно приема има на файл или файлов дескриптор

        ```bash
        echo "Hello" > file.txt # redirect stdout to file.txt
        echo "Hello" &> file.txt # redirect both stdout and stderr
        echo "Hello" > &1 # redirect stdout to stdout
        ```

    - | - отляво е команда, отдясно е команда

        ```bash
        echo "Hello" | grep "H" # pipe stdout of echo to stdin of grep
        ```

    - <<< - отляво е команда, отдясно е символен низ

        ```bash
        grep "H" <<< "Hello" # redirect string to stdin of grep
        ```

    - < - отляво е команда, отдясно е име на файл или файлов дескриптор

        ```bash
        grep "H" < file.txt # redirect file.txt to stdin of grep
        grep "H" <&1 # redirect stdin to stdin
        ```

    16.2. Какво правят?

    (аз така леко без да прочета по-надолу въпросите съм ги отговорил и останалите, ама за да сме честни ги пиша и тук)

    - \> - пренасочва stdout на командата отляво към файл или файлов дескриптор отдясно
    - | - пренасочва stdout на командата отляво към stdin на командата отдясно
    - <<< - пренасочва символен низ отдясно към stdin на командата отляво
    - < - пренасочва stdin на командата отляво към файл или файлов дескриптор отдясно

    16.3. Примерна употреба.

    ```bash
    find / -user $(whoami) 2 > /dev/null | wc -l
    stat -c '%h %n' */* | sort -n | tail -n 5
    cat <<< "Hello"
    grep "H" < file.txt
    ```

    16.4. Обяснение какво се случва в примера ви.

    ```bash
    find / -user $(whoami) 2 > /dev/null | wc -l
    ``` 

    - find - намира файлове в дадена директория
    - / - директорията, в която да търсим
    - -user $(whoami) - намира файлове, които са собственост на текущия потребител
    - 2 > /dev/null - пренасочва stderr на командата отляво към /dev/null
    - | - пренасочва stdout на командата отляво към stdin на командата отдясно
    - wc -l - преброява редовете на stdin

    ```bash
    stat -c '%h %n' */* | sort -n | tail -n 5
    ```

    - stat - извежда информация за файлове
    - -c '%h %n' - форматира изхода
    - ```*/*``` - всички файлове във всички директории
    - | - пренасочва stdout на командата отляво към stdin на командата отдясно
    - sort -n - сортира числово
    - tail -n 5 - извежда последните 5 реда

    ```bash
    cat <<< "Hello"
    ```

    - cat - извежда съдържанието на файл
    - <<< - пренасочва символен низ отдясно към stdin на командата отляво

    ```bash
    grep "H" < file.txt
    ```

    - grep - търси символен низ във файл
    - < - пренасочва stdin на командата отляво към файл или файлов дескриптор отдясно

17. За следните понятия - globs, brace expansion, shell substitution $():

    17.1. Обяснете синтаксисът.
    - globs - символи със специално значение, които могат да се expand-нат до множество от символи. Те са- `*`, `?`, `[]`. Като е object (file) aware
    - brace expansion - генерира поредица от низове `{test, test1, test2}` и интервали `{1..10}`. За разлика от globs, това е string aware, а не object aware (т.е. "плюе" всичко)
    - shell substitution `$({command})` - в `$()` слагаме команда или множество от команди и резултатът им се замества на мястото на $()

    - globs - шаблони за имена на файлове (wildcards) - * ? [ ]

        ```bash
        echo * # извежда всички файлове в текущата директория
        echo ? # извежда всички файлове с една буква в името
        echo [a-z] # извежда всички файлове с една малка буква в името
        ```

    - brace expansion - разширяване на скоби

        ```bash
        echo {1..5} # 1 2 3 4 5
        echo {a..z} # a b c d e f g h i j k l m n o p q r s t u v w x y z
        echo {1..5..2} # 1 3 5
        ```

    - shell substitution $() - изпълнява команда и връща изхода |

        ```bash
        echo $(ls) # извежда съдържанието на текущата директория
        ```

    17.2. С какво се заместват?

    - glob (object aware)
        - `*` - 0 или повече срещания на определен символ
        - `?` - произволен символ
        - `[]` - символ от изброените между скобите символи
    - brace expansion (not object aware) - замества се със списък от всички възможни комбинации с елементите в скобите и тези извън.
    - shell substitution - Изходът на командата/командите в `$()` се замества в конструкцията

    17.3. Дайте пример. Обяснете какво се случва в примера ви.

    ```bash
    echo {1..5} # 1 2 3 4 5
    echo {a..z} # a b c d e f g h i j k l m n o p q r s t u v w x y z
    echo {1..5..2} # 1 3 5
    
    echo $(ls) # извежда съдържанието на текущата директория

    echo * # извежда всички файлове в текущата директория
    ```

18. За всяка от конструкциите if, while, for:

    18.1. Обяснете синтаксисът и поведението на конструкцията
    
    if - изпълнява команди, ако условието е вярно

    ```bash
    if [[condition]] ; then # ако условието е вярно ще се изпълни команда 1
        command1
    elif [[condition]] ; then # ако първото условие не е вярно, ще се провери второто, ако е вярно ще се изпълни команда 2
        command2
    else # ако нито едно от горните условия не е вярно, ще се изпълни команда 3
        command3
    fi
    ```

    while - изпълнява команди, докато условието е вярно

    ```bash
    while [[condition]]; do # докато условието е вярно ще се изпълнява командата
        command
    done
    ```

    for - изпълнява команди за всеки елемент от списък

    ```bash
    for var in $(command); do # за всеки елемент от изхода на командата ще се изпълни команда
        command
    done
    ```

    ```bash
    for i in {n..m}; do # за всяка числова стойност от n до m ще се изпълни команда
        command
    done
    ```

    ```bash

    ```bash
    for i in list_n1 list_n2; do # за всеки елемент от списъка ще се изпълни команда
        command
    done
    ```

    18.2. Дайте пример + обяснение какво се случва в примера ви.
    - проверява дали файлът file.txt съществува и ако да, извежда съобщение

        ```bash
        if [[ -f file.txt ]]; then
            echo "file.txt exists"
        fi
        ```

    - безкраен цикъл, който извежда "Hello"

        ```bash
        while [[ true ]]; do
            echo "Hello"
        done
        ```

    - извежда числата от 1 до 5

        ```bash
        for i in {1..5}; do
            echo $i
        done
        ```
        
    *още примери може да видите в прикачените скриптове във този архив*


19. Как се декларира променлива в bash? Как се реферира? Дайте пример.

    Преди да кажем как се декларира променлива, трябва да разгледаме какви видове променливи има в bash. Те биват 2 вида:
    - Shell (local) - достъпни само в текущия shell

        ```bash
        var=value
        ```

    - Environmental - достъпни от всички shell-ове и процеси, стартирани от текущия shell (т.е. от децата)

        ```bash
        export v=str
        ```

    Като може да ги достъпим по следните начини:
    - `$var`
    - `${var}`

## Security Context на процеси и специални пермишъни

20. С какъв owner и group се създват новосъздадени процеси?

    Новосъздадените процеси се създават с owner - потребителя, който е стартирал процеса и за group - неговата текуща активна група

21. Какви ID-та се асоциират с процеси?

    Security контекст на процеси се състои от user & group (мета характеристики на процеса), както и 3 множества на id-та, които биват:
    - Real user id (по подразбиране е id-тото на създателя на този процес) & group id (активната група създателя на този процес)
    - Effective user id (по подразбиране е id-тото на създателя на този процес) & effective group id (активната група създателя на този процес)
    - Safe user id (по подразбиране е id-тото на създателя на този процес) & safe group id (активната група създателя на този процес)

    Но какво означават тези id-та?
    - Real - описва създателя на процеса (describe creator)
    - Effective - операционната система използва тези id-та, за да прецени реално този процес какви права има (permission control)
    - Safe - по време на своя живот процесите могат да променят своите effective id-та, т.е. процесите могат да разширят или намалят правата, които имат. Обаче се упражнява строг контрол за това кой процес може да повишава правата си - *capabilities*. Преди процеса да си смени effective id-тата, той ги записва в safe с цел възможно връщане назад в бъдеще, т.е. се пазят последните сменени effective id-та.

22. Колко специални пермишъна има? Кои са те, какво правят? (hint: file vs directory)

    Специалните права за достъп са 3:
    1. Set user id on execution, SUID
    2. Set group id on execution, SGID
    3. Stiky bit

    За файловете :
    1. SUID - има влияние само върху изпълними файлове, които не са скриптове. Той променя стойностите на effective id-то. Процесът се създава с EUID = id(owner(file)), т.е. по подразбиране EUID, което беше id-тото на създателя на този процес, се сменя
    2. SGID - прави същото като SUID само че за групи. EGID се сменя на групата, която притежава файла
    3. Stiky bit - в днешно време няма особено значение

    За директориите
    1. SUID - не прави нищо
    2. SGID - новите файлове и поддиректории наследят GID-то на parent директорията вместо тази на създателя им
    3. Stiky bit - прави дракони ✨


23. От какво зависи дали даден процес P ще може да чете/промени/изпълни даден файл? Дайте пример.

    Дали даден файл може да бъде изпълним, зависи от неговите права за достъп и от EID - effective id-то на файла.

    Users on system: kaloyan:kaloyan (1001, 1001), root:root (1000, 1000)

    ```
     ----------file-----------
    | filename: runnable_file |
    | permissions (8): 111    |
    | owner: kaloyan:kaloyan  |
    | SUID: unset             |
     -------------------------
    ```
    ```
     ----------file-----------
    | filename: interactable  |
    | permissions (8): 700    |
    | owner: root:root        |
     -------------------------
    ```
    kaloyan POV:
    ```bash
    $ ./runnable_file # tries to write to interactable file
    ```

    ```
     ----------proc-----------
    | proc: runnable_proc     |
    | EUID: 1001              |
    | EGID: 1001              |
     -------------------------
    ```

    стъпките, които се поемат са следните, първо се проверява EUID с UID на файла, които не съвпадат - 1000 !== 1001, след това би трябвало да се направи проверка за група и всички останали само че те нямат никакви права, затова (май) се пропускат. Иначе процесът би бил следният:

    1. EUID ?= UID
    2. EGID ?= GID
    3. Others permissions ??

    (modifed example from class)

## Източници 

Записки и малко интернет, за дните, в които съм отсъствал
