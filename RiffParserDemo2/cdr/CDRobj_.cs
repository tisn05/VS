using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace tisn.cdr
{
    class CDRobj_:RiffList
    {
        public CDRobj_(CDRFile p) : base(p) {
            Parent.ObjectCount++;
        }
    }
}
