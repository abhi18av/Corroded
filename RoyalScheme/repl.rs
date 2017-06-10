extern {
    static mut __stdinp : *mut __sFILE;
    static mut __stdoutp : *mut __sFILE;
    fn __swbuf(arg1 : i32, arg2 : *mut __sFILE) -> i32;
    fn printf(arg1 : *const u8, ...) -> i32;
    fn scm_eval(expression : isize) -> isize;
    fn scm_fatal(fmt : *mut u8, ...);
    fn scm_read(in_ : *mut __sFILE) -> isize;
    fn scm_write(out : *mut __sFILE, object : isize) -> i32;
}

enum __sFILEX {
}

#[derive(Copy)]
#[repr(C)]
pub struct __sbuf {
    pub _base : *mut u8,
    pub _size : i32,
}

impl Clone for __sbuf {
    fn clone(&self) -> Self { *self }
}

#[derive(Copy)]
#[repr(C)]
pub struct __sFILE {
    pub _p : *mut u8,
    pub _r : i32,
    pub _w : i32,
    pub _flags : i16,
    pub _file : i16,
    pub _bf : __sbuf,
    pub _lbfsize : i32,
    pub _cookie : *mut ::std::os::raw::c_void,
    pub _close : unsafe extern fn(*mut ::std::os::raw::c_void) -> i32,
    pub _read : unsafe extern fn(*mut ::std::os::raw::c_void, *mut u8, i32) -> i32,
    pub _seek : unsafe extern fn(*mut ::std::os::raw::c_void, isize, i32) -> isize,
    pub _write : unsafe extern fn(*mut ::std::os::raw::c_void, *const u8, i32) -> i32,
    pub _ub : __sbuf,
    pub _extra : *mut __sFILEX,
    pub _ur : i32,
    pub _ubuf : [u8; 3],
    pub _nbuf : [u8; 1],
    pub _lb : __sbuf,
    pub _blksize : i32,
    pub _offset : isize,
}

impl Clone for __sFILE {
    fn clone(&self) -> Self { *self }
}

#[no_mangle]
pub unsafe extern fn __sputc(
    mut _c : i32, mut _p : *mut __sFILE
) -> i32 {
    if {
           (*_p)._w = (*_p)._w - 1;
           (*_p)._w
       } >= 0i32 || (*_p)._w >= (*_p)._lbfsize && (_c as (u8) as (i32) != b'\n' as (i32)) {
        ({
             let _rhs = _c;
             let _lhs
                 = &mut *{
                             let _old = (*_p)._p;
                             (*_p)._p = (*_p)._p.offset(1isize);
                             _old
                         };
             *_lhs = _rhs as (u8);
             *_lhs
         }) as (i32)
    } else {
        __swbuf(_c,_p)
    }
}

fn main() {
    let ret = unsafe { _c_main() };
    ::std::process::exit(ret);
}

unsafe extern fn repl() {
    let mut p : i32;
    let mut object : isize;
    'loop1: loop {
        p = printf((*b"> \0").as_ptr());
        if p < 0i32 {
            scm_fatal((*b"repl: print failed\0").as_ptr() as (*mut u8));
        }
        object = scm_read(__stdinp);
        object = scm_eval(object);
        p = scm_write(__stdoutp,object);
        if p < 0i32 {
            scm_fatal((*b"repl: print failed\0").as_ptr() as (*mut u8));
        }
        p = printf((*b"\n\0").as_ptr());
        if !(p < 0i32) {
            continue;
        }
        scm_fatal((*b"repl: print failed\0").as_ptr() as (*mut u8));
    }
}

#[no_mangle]
pub unsafe extern fn _c_main() -> i32 {
    printf(
        (*b"Welcome to Royal Scheme. Ctrl-c to exit.\n\0").as_ptr()
    );
    repl();
    0i32
}
