✨ Операционни системи ✨
=========================
Документация за предмета "Опрерационни системи" (11-ти клас) за учебната ***ТУЕС, 2022/23*** година

## Съдържание
1. [Macros | Макроси](#macros--макроси)
1. [Basic Linux команди](#basic-linux-команди)
1. [Файлове и файлови дескриптори](#файлове-и-файлови-дескриптори)

## Macros | Макроси
## Basic Linux команди 
- ```whoami``` - "Who am I? 😯" - връща името на текущия потребител

  <img width="123" alt="image" src="https://user-images.githubusercontent.com/46886807/197382296-0bc4246a-b612-4878-8378-f7bad3d781bd.png">

- ```pwd``` - "Print work directory" - посочва директорията, в която се намираме

  <img width="184" alt="image" src="https://user-images.githubusercontent.com/46886807/197382316-11b87ed5-0ee7-4708-85c8-2c75ca009586.png">

- ```ls``` - "List" - дава информация за това какви файлове и директории има в текущата директория

  <img width="426" alt="image" src="https://user-images.githubusercontent.com/46886807/197382371-47447a0b-39f7-4176-8fb1-7342a86cb0ee.png">

- ```mkdir <name>``` - "Make directory" - създава директория на име <name> там, където се намираме

  <img width="569" alt="image" src="https://user-images.githubusercontent.com/46886807/197382405-844e65cb-7a1a-4139-977f-1ed09c8173dd.png">

- ```mv``` <src> <dest> - "Move" - премества избрания файл на дадена локация (може да се използва и за преименуване на дадения файл) (може да се използва и за директории)

- ```cp <src> <dest>``` - "Copy" - копира избрания файл на дадена локация (може да се използва и за директории)

- ```cd``` - "Change Directory" - премества потебителя в дадена директория
  - ```cd <name>``` - премества потребитя в подадената от него директория с път <name>
  
    <img width="275" alt="image" src="https://user-images.githubusercontent.com/46886807/197382680-e1eecf1e-1faa-4115-98d2-edcdbeb9a687.png">
  
  - ```cd ..``` - премества потребителя с една директория нагоре (във "файловото дърво" - file structure tree)
  
    <img width="297" alt="image" src="https://user-images.githubusercontent.com/46886807/197382485-7445557e-a495-4734-b736-a3e079909080.png">
  
  - ```cd .``` - "премества" потребителя в текущата директория
  
    <img width="201" alt="image" src="https://user-images.githubusercontent.com/46886807/197382505-23e65f9a-8107-4f9c-85b5-b87dc1ca6717.png">
  
  - ```cd /``` - премества потребителя в **ROOT** директорията
  
    <img width="222" alt="image" src="https://user-images.githubusercontent.com/46886807/197382519-b3a3ee7f-9c75-4b19-ba8c-d149ae809df6.png">
 
  - ```cd ~``` - премества потребителя в съответната за него **HOME** (потребителска) директория 
  
    <img width="88" alt="image" src="https://user-images.githubusercontent.com/46886807/197382553-000b7245-c204-4d96-acd4-b3be4eaf6e30.png">

- ```touch <name>``` - създава файл на име <name> без да го отваря

  <img width="405" alt="image" src="https://user-images.githubusercontent.com/46886807/197382717-3a3b8813-2b60-4ef0-9596-845f7d65fdb3.png">

- ```cat <name>``` - "Concatenate" - извежда (конкатенира) стойността на даден файл

  <img width="383" alt="image" src="https://user-images.githubusercontent.com/46886807/197383141-32c9e09b-73f8-4b32-9ff5-ad4d3c7ccbb4.png">

- ```man``` - "Manual page" - документация за функции и команди
  - ```man <name>``` - "General commands menu" - документация за обикновени функции и команди

    <img width="682" alt="Screenshot 2022-10-23 at 11 39 22 AM" src="https://user-images.githubusercontent.com/46886807/197382961-c0dc8218-7305-4786-ad48-7035a7ad2c66.png">

  - ```man 2 <name>``` - System calls menu - документация за системни функции и команди (System Calls - системни повиквания)
  
    <img width="682" alt="Screenshot 2022-10-23 at 11 46 26 AM" src="https://user-images.githubusercontent.com/46886807/197382984-8007049c-3202-4fa2-80c7-b4b200efa527.png">
  
- ```vim <name>``` - vim (което означава vi (стар текстов редактор) improved) е изключително ***основан*** текстов редактор за терминал (btw води се за един от най-ефикасните и мощни редактори) - чрез тази команда или създаваме и отваряме за редактиране файл <name>, или само отваряме вече съществуващ файл <name>

     <img width="389" alt="image" src="https://user-images.githubusercontent.com/46886807/197383057-d83a8eb4-20b7-4ec8-9fb9-a04914f2d493.png">
<img width="682" alt="image" src="https://user-images.githubusercontent.com/46886807/197383063-8b13df9d-a18b-42bf-9d19-57fa60526c09.png">

- ```nano <name>``` - изключително опростен текстов редактор, който "върши работата". Най-често се използва за редактиране на кофигурационни файлове. Тази команда върши всичко аналогично на vim само че с nano

     <img width="389" alt="image" src="https://user-images.githubusercontent.com/46886807/197383085-c31c8cec-1e65-4196-bf66-1d80c608cbaa.png">
<img width="682" alt="image" src="https://user-images.githubusercontent.com/46886807/197383105-e98f1373-8adf-420d-ae85-712af2b59d48.png">
  
## Файлове и файлови дескриптори
Функции, които ще учим за файлове:
- open
- close
- read
- write
- lseek
