namespace Zhangyue.iReader.EBK2
{
    using System;
    using System.Collections.Generic;

    public class BookInfo
    {
        private string _bookName;
        private List<BookItems> _chapterlist = new List<BookItems>();
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

        public List<BookItems> ChaterList
        {
            get
            {
                return this._chapterlist;
            }
            set
            {
                this._chapterlist = value;
            }
        }

        public byte[] Picture
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

