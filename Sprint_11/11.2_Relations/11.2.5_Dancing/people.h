#pragma once

#include <string>

class Person;

// Наблюдатель за состоянием человека.
class PersonObserver {
public:
    // Этот метод вызывается, когда меняется состояние удовлетворённости человека
    virtual void OnSatisfactionChanged(Person& person, int old_value, int new_value) {
        (void)person;
        (void)old_value;
        (void)new_value;
    }

protected:
    // Класс PersonObserver не предназначен для удаления напрямую
    ~PersonObserver() = default;
};

/*
    Человек.
    При изменении уровня удовлетворённости уведомляет
    связанного с ним наблюдателя
*/
class Person {
public:
    Person(const std::string& name, int age) :
        name_(name),
        age_(age)
    {
        
    }

    virtual ~Person() = default;

    int GetSatisfaction() const {
        return satisfaction_;
    }

    const std::string& GetName() const {
        return name_;
    }

    // «Привязывает» наблюдателя к человеку. Привязанный наблюдатель
    // уведомляется об изменении уровня удовлетворённости человека
    // Новый наблюдатель заменяет собой ранее привязанного
    // Если передать nullptr в качестве наблюдателя, это эквивалентно отсутствию наблюдателя
    void SetObserver(PersonObserver* observer) {
        observer_ = observer;
    }

    int GetAge() const {
        return age_;
    }

    // Увеличивает на 1 количество походов на танцы
    // Увеличивает удовлетворённость на 1
    void Dance() {
        dance_count_ += 1;
        OnDansing();
    }

    int GetDanceCount() const {
        return dance_count_;
    }

    // Прожить день. Реализация в базовом классе ничего не делает
    virtual void LiveADay() {
        // Подклассы могут переопределить этот метод
    }
protected:
    virtual void OnDansing(){
        ChangeSatisfaction(1);
    }

    void ChangeSatisfaction(int pleasure){
        satisfaction_ += pleasure;
        if(observer_ != nullptr){
            observer_->OnSatisfactionChanged(*this, satisfaction_ - pleasure, satisfaction_ );
        }
    }

private:
    std::string name_;
    PersonObserver* observer_ = nullptr;
    int satisfaction_ = 100;
    int age_;
    int dance_count_ = 0;
};

// Рабочий.
// День рабочего проходит за работой
class Worker : public Person{
public:
    Worker(const std::string& name, int age) :
        Person(name, age)
    {
        // Напишите недостающий код
    }

    // Рабочий старше 30 лет и младше 40 за танец получает 2 единицы удовлетворённости вместо 1

    // День рабочего проходит за работой

    // Увеличивает счётчик сделанной работы на 1, уменьшает удовлетворённость на 5
    void Work() {
        work_done_ +=1;
        Person::ChangeSatisfaction(-5);
    }

    // Возвращает значение счётчика сделанной работы
    int GetWorkDone() const {
        return work_done_;
    }

    void LiveADay() override {
        Work();
    }


protected:
    void OnDansing() override{
        if(GetAge()> 30 && GetAge()<40){
            ChangeSatisfaction(2);   
        } else{
            ChangeSatisfaction(1);
        }
    }
private:
    int work_done_ = 0;
};

// Студент.
// День студента проходит за учёбой
class Student : public Person {
public:
    Student(const std::string& name, int age) :
        Person(name, age)
    {
        // Напишите недостающий код
    }

    // День студента проходит за учёбой

    // Учёба увеличивает уровень знаний на 1, уменьшает уровень удовлетворённости на 3
    void Study() {
        knowledge_ += 1;
        ChangeSatisfaction(-3);
    }

    // Возвращает уровень знаний
    int GetKnowledgeLevel() const {
        return knowledge_;
    }
    void LiveADay() override {
        Study();
    }

private:
    int knowledge_ = 0;
};