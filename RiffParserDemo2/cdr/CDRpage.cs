using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace tisn.cdr
{
    public class CDRpage : RiffList
    {
        public CDRpage(CDRFile p)
            : base(p)
        {
            Parent.PageCount++;
        }
    }
}
