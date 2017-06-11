extern {
    static mut __stdinp : *mut __sFILE;
    static mut __stdoutp : *mut __sFILE;
    fn __swbuf(arg1 : i32, arg2 : *mut __sFILE) -> i32;
    fn freopen(
        arg1 : *const u8, arg2 : *const u8, arg3 : *mut __sFILE
    ) -> *mut __sFILE;
    fn printf(arg1 : *const u8, ...) -> i32;
    fn scanf(arg1 : *const u8, ...) -> i32;
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
     _extra : *mut __sFILEX,
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

#[no_mangle]
pub unsafe extern fn _c_main() -> i32 {
    freopen((*b"input.txt\0").as_ptr(),(*b"rt\0").as_ptr(),__stdinp);
    freopen((*b"output.txt\0").as_ptr(),(*b"wt\0").as_ptr(),__stdoutp);
    let mut a : i32;
    let mut b : i32;
    scanf(
        (*b"%d%d\0").as_ptr(),
        &mut a as (*mut i32),
        &mut b as (*mut i32)
    );
    printf((*b"%d\n\0").as_ptr(),a + b);
    0i32
}
