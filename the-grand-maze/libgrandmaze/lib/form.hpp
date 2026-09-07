#ifndef FORM_H
#define FORM_H

#include <string>
#include <vector>
#include <iterator>
#include <functional>

#include <ncurses.h>

namespace GrandMaze
{
    // Form operations
    enum FormOp
    {
        NoOp,
        Push,
        Pop,
        NextField,
        PrevField,
        SubmitForm,
        CancelForm,
    };

    // FormCallback is a callback function that takes an int
    // and returns a FormOp 
    typedef std::function<FormOp (int)> FormCallback;

    class Field
    {
    friend class Form;
    protected:
        int x, y, height, width;
        bool showName;
        std::string name;
        std::string value;

    public:
        Field(int x, int y, int height, int width, std::string name, 
            bool showName, std::string value);
        Field(int x, int y, int height, int width, std::string name, 
            std::string value);
        Field(int x, int y, int height, int width, std::string name);
        
        std::string getValue(void);
    };

    class Form
    {
    private:
        WINDOW *win;
        std::vector<Field*> fields;
        std::string title;
        std::vector<Field*>::iterator currentField;

        // Draw the form
        void draw(void);
        // Draw fields in the form
        void drawFields(void);
        // Draw a single field in the form
        void drawField(Field *field);

    public:
        Form(WINDOW *win, std::string title);

        // Add a new field to the form
        void addField(Field *field);
        // Display and get input from the form
        void show(FormCallback callback);
    };
};

#endif
