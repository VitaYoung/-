using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Todos.Models
{
    class TodoItem
    {

        public int id { get; set; }

        public string title { get; set; }

        public string description { get; set; }

        public bool completed { get; set; }

        public DateTime date { get; set; }

        public TodoItem(int id, string title, string description, DateTime date)
        {
            this.id = id; 
            this.title = title;
            this.description = description;
            this.completed = false;
            this.date = date;
        }
    }
}
