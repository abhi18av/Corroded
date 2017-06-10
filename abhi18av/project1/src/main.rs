extern "C" {
    fn __swbuf(arg1: i32, arg2: *mut __sFILE) -> i32;
    fn printf(arg1: *const u8, ...) -> i32;
}

enum __sFILEX {
}

#[derive(Copy)]
#[repr(C)]
pub struct __sbuf {
    pub _base: *mut u8,
    pub _size: i32,
}

impl Clone for __sbuf {
    fn clone(&self) -> Self {
        *self
    }
}

#[derive(Copy)]
#[repr(C)]
pub struct __sFILE {
    _p: *mut u8,
    pub _r: i32,
    pub _w: i32,
    pub _flags: i16,
    pub _file: i16,
    pub _bf: __sbuf,
    pub _lbfsize: i32,
    pub _cookie: *mut ::std::os::raw::c_void,
    pub _close: unsafe extern "C" fn(*mut ::std::os::raw::c_void) -> i32,
    pub _read: unsafe extern "C" fn(*mut ::std::os::raw::c_void, *mut u8, i32) -> i32,
    pub _seek: unsafe extern "C" fn(*mut ::std::os::raw::c_void, isize, i32) -> isize,
    pub _write: unsafe extern "C" fn(*mut ::std::os::raw::c_void, *const u8, i32) -> i32,
    pub _ub: __sbuf,
    _extra: *mut __sFILEX,
    pub _ur: i32,
    pub _ubuf: [u8; 3],
    pub _nbuf: [u8; 1],
    pub _lb: __sbuf,
    pub _blksize: i32,
    pub _offset: isize,
}

impl Clone for __sFILE {
    fn clone(&self) -> Self {
        *self
    }
}

#[no_mangle]
pub unsafe extern "C" fn __sputc(mut _c: i32, mut _p: *mut __sFILE) -> i32 {
    if {
        (*_p)._w = (*_p)._w - 1;
        (*_p)._w
    } >= 0i32 || (*_p)._w >= (*_p)._lbfsize && (_c as (u8) as (i32) != b'\n' as (i32)) {
        ({
            let _rhs = _c;
            let _lhs = &mut *{
                let _old = (*_p)._p;
                (*_p)._p = (*_p)._p.offset(1isize);
                _old
            };
            *_lhs = _rhs as (u8);
            *_lhs
        }) as (i32)
    } else {
        __swbuf(_c, _p)
    }
}

fn main() {
    let ret = unsafe { _c_main() };

    print!("Hello, Rust");

    ::std::process::exit(ret);
}

#[no_mangle]
pub unsafe extern "C" fn _c_main() -> i32 {
    printf((*b"Hello, World!\n\0").as_ptr());
    let mut i: i32;
    let mut A: [f64; 5] =
        [1i32 as (f64), 2i32 as (f64), 3i32 as (f64), 4i32 as (f64), 5i32 as (f64)];
    i = 0i32;
    'loop1: loop {
        if !(i < 5i32) {
            break;
        }
        printf(
            (*b"element\xE2\x90\xA3%d\xE2\x90\xA3is\xE2\x90\xA3%g,\xE2\x90\xA3\tits\xE2\x90\xA3squa\0").as_ptr(
            ),
            i,
            A[i as (usize)],
            A[i as (usize)] * A[i as (usize)]
        );
        i = i + 1;
    }
    0i32
}
