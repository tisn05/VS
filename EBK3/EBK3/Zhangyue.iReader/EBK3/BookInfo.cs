namespace Zhangyue.iReader.EBK3
{
    using System;
    using System.Collections.Generic;

    public class BookInfo
    {
        private string _bookName;
        private string _author;
        private List<BookItems> _m_chapterlist = new List<BookItems>();
        private byte[] _pic;
        private string _text = "";

        public string BookName
        {
            get
            {
                return this._bookName;
            }
            set
            {
                this._bookName = value;
            }
        }

        public string Author
        {
            get
            {
                return this._author;
            }
            set
            {
                this._author = value;
            }
        }

        public List<BookItems> ChaterList
        {
            get
            {
                return this._m_chapterlist;
            }
            set
            {
                this._m_chapterlist = value;
            }
        }

        public byte[] Pic
        {
            get
            {
                return this._pic;
            }
            set
            {
                this._pic = value;
            }
        }

        public string Text
        {
            get
            {
                return this._text;
            }
            set
            {
                this._text = value;
            }
        }
    }
}

