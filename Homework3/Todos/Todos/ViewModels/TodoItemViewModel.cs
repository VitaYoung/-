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
            this.allItems.Add(new Models.TodoItem("test1", "aaaa", DateTime.Now));
            this.allItems.Add(new Models.TodoItem("test2", "bbbb", DateTime.Now));
        }

        public void AddTodoItem(string title, string description, DateTime date)
        {
            this.allItems.Add(new Models.TodoItem(title, description, DateTime.Now));
        }

        public void RemoveTodoItem(string id)
        {
            this.allItems.Remove(selectedItem);
            this.selectedItem = null;
        }

        public void UpdateTodoItem(string id, string title, string description, DateTime date)
        {
            this.selectedItem.title = title;
            this.selectedItem.description = description;
            this.selectedItem.date = date;
            this.selectedItem = null;
        }

    }
}
