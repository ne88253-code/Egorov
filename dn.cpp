#include <iostream> 
#include<vector>
using namespace std;
//ключевые понятия (принципы) ООП:
//1) наследование
//2) полиморфизм
//3) абстракция
//4) инкапсуляция(геттер  и сеттер)

// public - публичный (доступен внутри класса,внутри наследника и в основном потоке программы)
//protected - защищенный (можно изменять в исходном классе и классах наследниках)
// private - приватный (доступен только в исходном классе)
//private - не наследуется
class NPC
{
protected:
 string name{ "npc" };
 unsigned int damage{ 2 };
 unsigned int health{ 5 };
 short lvl = 1;
 unsigned int armor = 2;
private: bool isEnemy = true;
public:
 unsigned int GetDamage() { return damage; };
 unsigned int GetHealth() { return health; }; //геттер
 void SetHealth(unsigned int health) { this->health = health; };//сеттер
 virtual void GetInfo()
 {
  cout << "имя: " << name << endl;
  cout << "здоровье: " << health << endl;
  cout << "урон: " << damage << endl;
  cout << "уровень: " << lvl << endl;
  cout << "броня: " << armor << endl;
 };
 // создать NPC нельзя, поэтому он виртуальный.
 virtual void Create() {};//хотя бы 1 метод виртуальный, значит весь класс виртуальный
 void LvlUp()
 {
  cout << name << " повысил новый уровень";
  Relaculate();
 }
 void Relaculate() // кастомизировать для война и волшебника в зависимости от инт/силы 
 {
  damage += (1 + lvl * 0.1);
  health += (1 + lvl * 0.1);
  armor += (1 + lvl * 0.1);
 }

 friend void TakeDamage(NPC&, unsigned int damage);

 virtual ~NPC() = default;
};
struct Weapon
{
 string name{ "weapon" };
 unsigned int damage{ 1 };
};

class Warrior : virtual public NPC
{
protected:
 short strength{ 21 };
 vector<Weapon> weapons;
public:
 // конструктор по умолчанию
 Warrior()
 {
  //cout << "конструктор война" << endl;
  damage = 20;
  health = 30;
  armor = 15;
  
 }
 //кастомный конструктор
 Warrior(string name, unsigned int lvl)
 {
  this->name = name; // такой способ задания поля уместен только для сеттер
  for (size_t i = 0; i < lvl; i++)
  {
   LvlUp();
  }

  //this->lvl = lvl; //this - указывает на конкретный экземпляр класса
 }
 void Create() override
 {
  cout << "Вы создали война\nЗадайте имя игрока\n";
  cin >> name;

  GetInfo();
  GetWeapon();
 };
 void GetInfo() override
 {
  cout << "имя: " << name << endl;
  cout << "здоровье: " << health << endl;
  cout << "урон: " << damage << endl;
  cout << "сила: " << strength << endl;
 };
 void GetWeapon()
 {
  Weapon weapon;
  weapon.damage = 1;
  weapon.name = "кулаки";
  weapons.push_back(weapon);

  cout << name << " взял в руки оружие " << weapons[0].name << endl;
  cout << " добавка к урону = " << weapons[0].damage << endl;
 };
 ~Warrior()//деструктор(вызывается сам в момент высвобождения)
 {
  cout << name << " пол смертью храбрых" << endl;
 }
};

struct Spell
{
 string name{ "spell" };
 unsigned int damage{ 1 };
};

class Wizard : virtual public NPC
{
protected:
 short intellect{ 29 };
 vector<Spell> spells;
public:
 Wizard()
 {
  //cout << "конструктор волшебник" << endl;
  damage = 27;
  health = 21;
  armor = 10;
  
 }
 void Create() override
 {
  cout << "Вы создали волшебник\nЗадайте имя игрока\n";
  cin >> name;

  GetInfo();
  LearnSpell();
 };
 void GetInfo() override
 {
  cout << "имя: " << name << endl;
  cout << "здоровье: " << health << endl;
  cout << "урон: " << damage << endl;
  cout << "интеллект: " << intellect << endl;
 };
 void LearnSpell()
 {
  Spell spell;
  spell.damage = 2;
  spell.name = "вспышка";
  spells.push_back(spell);

  cout << name << " изучил заклинание " << spells[0].name << endl;
  cout << " добавка к урону = " << spells[0].damage << endl;
 };
 ~Wizard()
 {
  cout << name << "испускает дух" << endl;
 }
};
class Evil : public NPC
{
public:
 Evil()
 {
  name = "Злодей";
  health = 10;
  damage = 10;
  armor = 3;
 }
 Evil(string name) : Evil() //делегирование конструктора(вначале вызовется базовый)
 {
  this->name = name;

 }
 Evil(string name, unsigned int damage) : Evil(name)
 {
  this->damage = damage;

 }
 Evil(string name, unsigned int damage, unsigned int health) : Evil(name, damage)
 {
  this->health =
health;

 }
 Evil(string name, unsigned int damage, unsigned int health, unsigned intarmor) : Evil(name, damage, health)
 {
  this->armor = armor;

 }
 //придумать интересный детруктор для злодеев
};
//множественное наследование
class Paladin : public Warrior, public Wizard
{
public:
 Paladin() {
  intellect = 25;
  strength = 19;
  health = 25;
  damage = 25;
  Create();
 }
 void Create() override
 {
  //реализовать в NPC(храните название класса в каком то поле)
  cout << "Вы создали Паладина\nЗадайте имя игрока\n";
  cin >> name;

  GetInfo();
  GetWeapon();
  LearnSpell();
 };
 void GetInfo() override
 {
  Warrior::GetInfo();
  cout << "интеллект: " << intellect << endl;
 };
 ~Paladin()
 {
  cout << "Отправляется к праотцам" << endl;
 }
};

//дружественные классы
class Player
{
private:
 unique_ptr<NPC>currentCharacter{nullptr};
public:
 void Create(unique_ptr<NPC> character)
 {
  currentCharacter = move(character);
  currentCharacter->Create();
 }

 NPC* GetCharacter()
 {
  return currentCharacter.get();
 }
};
//дружественная к NPC функция - может использовать даже private поля и методы 
void TakeDamage(NPC* npc, unsigned int damage)
{
 npc->SetHealth(npc->health - damage);
 cout << "Вам нанесли урон: " << damage << endl;
 cout << "Оставшееся здоровье = " << npc->health;
}
int main()
{
 setlocale(LC_ALL, "Ru");
 
 Player player;
 cout << "Присядь путник у костра и расскажи, кто ты: " << endl;
 cout << "\t1 - воин\n\t2 - волшебник\n\t3 - паладин" << endl;
 short choice = 0;
 cin >> choice;
 switch (choice) 
 {
 case 1:
  player.Create(make_unique<Warrior>());
  break;
 case 2:
  player.Create(make_unique<Wizard>());
  break;
 case 3:
  player.Create(make_unique<Paladin>());
  break;
 default:
  cout << "Таких героев ещё не было в наших краях.\nПопытай удачу позже.";
 }

 TakeDamage(player.GetCharacter(), 5);

 // превратить злодеев в вектор(указателей,умных)
 //Evil evil, evil1, evil2("Кабанчик"), evil3("Гнолл", 12),
  //evil4("Гнолл Дробитель", 15, 20), evil5("Дракон", 50, 100, 200);
 //evil.GetInfo();
 return 0;
}
