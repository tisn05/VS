namespace Zhangyue.iReader.EBK3
{
    using System;

    public class BookItems
    {
        private int _index;
        private int _length;
        private string _name = "";

        public BookItems(string myname, int myindex, int mylength)
        {
            this._name = myname;
            this._index = myindex;
            this._length = mylength;
        }

        public int Index
        {
            get
            {
                return this._index;
            }
            set
            {
                this._index = value;
            }
        }

        public int Length
        {
            get
            {
                return this._length;
            }
            set
            {
                this._length = value;
            }
        }

        public string Name
        {
            get
            {
                return this._name;
            }
            set
            {
                this._name = value;
            }
        }
    }
}

