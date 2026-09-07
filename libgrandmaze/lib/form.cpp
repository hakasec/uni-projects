#include <functional>
#include <string>

#include <ncurses.h>

#include "form.hpp"

using namespace GrandMaze;

Form::Form(WINDOW *win, std::string title)
    : win(win), title(title)
{

}

void Form::draw(void)
{
    wclear(win);
    // get max column
    int maxcol = getmaxx(win);
    box(win, '|', '-');
    // insert title
    mvwaddstr(win, 1, (maxcol/2)-(title.size()/2), title.c_str());
    for (int i = 1; i < maxcol-1; i++)
        // add title underline
        mvwaddch(win, 2, i, '-');

    drawFields();
    wrefresh(win);
}

void Form::drawFields(void)
{
    for (auto field : fields)
    {
        drawField(field);
    }
}

void Form::drawField(Field *field)
{
    int boxSize = field->width;
    int x = field->x + 1;
    int y = field->y + 3;

    // if showName, display the name next to the field
    if (field->showName)
    {
        boxSize -= field->name.size() + 1;
        mvwaddstr(win, y+1, x, field->name.c_str());
        waddch(win, ' ');
    }

    x = getcurx(win);

    // draw field box
    for (int i = 0; i < boxSize; i++)
    {
        char mid = i == 0 || i == boxSize-1 ? '|' : ' ';
        char top = i == 0 || i == boxSize-1 ? '+' : '-';
        mvwaddch(win, y, x+i, top);
        mvwaddch(win, y+field->height+1, x+i, top);
        for (int j = 1; j <= field->height; j++)
        {
            mvwaddch(win, y+j, x+i, mid);
        }
    }

    // draw text if not empty
    if (field->value.size() > 0)
    {
        int textSize = boxSize - 3;
        char *textToDiplay = new char[textSize+1];
        // if text is bigger than the field
        if (textSize < field->value.size())
        {
            // tail text so that the it fits in the field
            field->value.copy(
                textToDiplay,
                textSize,
                field->value.size() - textSize
            );
            textToDiplay[textSize] = '\0';
        }
        else
        {
            // the whole string fits, so copy direct
            field->value.copy(textToDiplay, field->value.size());
            textToDiplay[field->value.size()] = '\0';
        }
        // insert tailed text
        mvwaddstr(win, y+1, x+1, textToDiplay);
        // free memory
        delete textToDiplay;

        // if field being draw is the current focused one, then add cursor
        if (*currentField == field)
            waddch(win, '_' | A_BLINK);
    }
    else
    {
        if (*currentField == field)
            mvwaddch(win, y+1, x+1, '_' | A_BLINK);
    }
}

void Form::addField(Field *field)
{
    fields.push_back(field);
}

void Form::show(FormCallback callback)
{
    // set current field as the first field
    currentField = fields.begin();
    draw();

    do
    {
        // get user input
        int c = wgetch(win);
        // pass user input to form callback
        FormOp op = callback(c);

        // process returned operation
        if (op == NoOp)
        {
            // do nothing
            continue;
        }
        else if (op == Push)
        {
            // add last inputted char to value of the current field
            (*currentField)->value.push_back(c);
            drawField(*currentField);
        }
        else if (op == Pop)
        {
            // if there is a value to delete from
            if ((*currentField)->value.size() > 0)
            {
                // delete last char from value
                (*currentField)->value.pop_back();
                drawField(*currentField);
            }
        }
        else if (op == NextField)
        {
            // if the next field in the iterator isn't the end 
            if (currentField+1 != fields.end())
            {
                // increment to next field
                currentField++;
                drawField(*(currentField-1));
                drawField(*currentField);
            }
        }
        else if (op == PrevField)
        {
            // if we aren't at the first field
            if (currentField != fields.begin())
            {
                // decrement to previous field
                currentField--;
                drawField(*(currentField+1));
                drawField(*currentField);
            }
        }
        else if (op == SubmitForm)
        {
            // return value to calling function
            break;
        }
        else if (op == CancelForm)
        {
            // erase all values and return to calling function
            for (auto field : fields)
            {
                field->value.erase();
            }
            break;
        }
    }
    while (true);
}

Field::Field(int x, int y, int height, int width, std::string name, 
             bool showName, std::string value)
    : x(x), y(y), height(height), width(width), name(name), 
      showName(showName), value(value)
{
    
}

Field::Field(int x, int y, int height, int width, std::string name, 
             std::string value)
    : Field(x, y, height, width, name, false, value)
{
    
}

Field::Field(int x, int y, int height, int width, std::string name)
    : Field(x, y, height, width, name, false, "")
{

}

std::string Field::getValue(void)
{
    return value;
}
