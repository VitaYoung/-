using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Todos.ViewModels
{
    class TodoItemViewModel
    {
        private ObservableCollection<Models.TodoItem> allItems = new ObservableCollection<Models.TodoItem>();
        public ObservableCollection<Models.TodoItem> AllItems { get { return this.allItems; } }

        private Models.TodoItem selectedItem = default(Models.TodoItem);
        public Models.TodoItem SelectedItem { get { return selectedItem; } set { this.selectedItem = value; }  }

        public TodoItemViewModel()
        {
            using (var statement = App.conn.Prepare("SELECT* FROM Message"))
            {
                if (statement.ColumnCount != 0)
                {
                    while (statement.Step() == SQLitePCL.SQLiteResult.ROW)
                    {
                        this.AddTodoItem((int)(long)statement[0], (string)statement[1], (string)statement[2], Convert.ToDateTime(statement[3]));
                    }
                }
            }
        }

        public void AddTodoItem(int id, string title, string description, DateTime date)
        {

            this.allItems.Add(new Models.TodoItem(id, title, description, date));
        }

        public void RemoveTodoItem(int id)
        {
            this.allItems.Remove(selectedItem);
            this.selectedItem = null;
        }

        public void UpdateTodoItem(int id, string title, string description, DateTime date)
        {
            this.selectedItem.title = title;
            this.selectedItem.description = description;
            this.selectedItem.date = date;
            this.selectedItem = null;
        }

    }
}
