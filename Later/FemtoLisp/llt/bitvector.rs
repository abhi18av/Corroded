extern {
    fn memset(
        __b : *mut ::std::os::raw::c_void, __c : i32, __len : usize
    ) -> *mut ::std::os::raw::c_void;
    fn realloc(
        __ptr : *mut ::std::os::raw::c_void, __size : usize
    ) -> *mut ::std::os::raw::c_void;
}

#[no_mangle]
pub unsafe extern fn bitvector_resize(
    mut b : *mut u32,
    mut oldsz : usize,
    mut newsz : usize,
    mut initzero : i32
) -> *mut u32 {
    let mut p : *mut u32;
    let mut sz
        : usize
        = (newsz.wrapping_add(31usize) >> 5i32).wrapping_mul(
              ::std::mem::size_of::<u32>()
          );
    p = realloc(b as (*mut ::std::os::raw::c_void),sz) as (*mut u32);
    if p == 0i32 as (*mut ::std::os::raw::c_void) as (*mut u32) {
        0i32 as (*mut ::std::os::raw::c_void) as (*mut u32)
    } else {
        if initzero != 0 && (newsz > oldsz) {
            let mut osz
                : usize
                = (oldsz.wrapping_add(31usize) >> 5i32).wrapping_mul(
                      ::std::mem::size_of::<u32>()
                  );
            memset(
                &mut *p.offset(
                          osz.wrapping_div(::std::mem::size_of::<u32>()) as (isize)
                      ) as (*mut u32) as (*mut ::std::os::raw::c_void),
                0i32,
                sz.wrapping_sub(osz)
            );
        }
        p
    }
}

#[no_mangle]
pub unsafe extern fn bitvector_new(
    mut n : usize, mut initzero : i32
) -> *mut u32 {
    bitvector_resize(
        0i32 as (*mut ::std::os::raw::c_void) as (*mut u32),
        0usize,
        n,
        initzero
    )
}

#[no_mangle]
pub unsafe extern fn bitvector_nwords(mut nbits : usize) -> usize {
    nbits.wrapping_add(31usize) >> 5i32
}

#[no_mangle]
pub unsafe extern fn bitvector_set(
    mut b : *mut u32, mut n : usize, mut c : u32
) {
    if c != 0 {
        let _rhs = 1i32 << (n & 31usize);
        let _lhs = &mut *b.offset((n >> 5i32) as (isize));
        *_lhs = *_lhs | _rhs as (u32);
    } else {
        let _rhs = !(1i32 << (n & 31usize));
        let _lhs = &mut *b.offset((n >> 5i32) as (isize));
        *_lhs = *_lhs & _rhs as (u32);
    }
}

#[no_mangle]
pub unsafe extern fn bitvector_get(
    mut b : *mut u32, mut n : usize
) -> u32 {
    *b.offset(
         (n >> 5i32) as (isize)
     ) & (1i32 << (n & 31usize)) as (u32)
}

unsafe extern fn ntz(mut x : u32) -> i32 {
    let mut n : i32;
    if x == 0u32 {
        32i32
    } else {
        n = 1i32;
        if x & 0xffffu32 == 0u32 {
            n = n + 16i32;
            x = x >> 16i32;
        }
        if x & 0xffu32 == 0u32 {
            n = n + 8i32;
            x = x >> 8i32;
        }
        if x & 0xfu32 == 0u32 {
            n = n + 4i32;
            x = x >> 4i32;
        }
        if x & 0x3u32 == 0u32 {
            n = n + 2i32;
            x = x >> 2i32;
        }
        (n as (u32)).wrapping_sub(x & 1u32) as (i32)
    }
}

#[no_mangle]
pub unsafe extern fn bitvector_next(
    mut b : *mut u32, mut n0 : usize, mut n : usize
) -> u32 {
    let mut _currentBlock;
    if n0 >= n {
        n as (u32)
    } else {
        let mut i : u32 = (n0 >> 5i32) as (u32);
        let mut nb : u32 = (n0 & 31usize) as (u32);
        let mut nw : u32 = (n.wrapping_add(31usize) >> 5i32) as (u32);
        let mut w : u32;
        if i < nw.wrapping_sub(1u32) || n & 31usize == 0usize {
            w = *b.offset(i as (isize)) >> nb;
        } else {
            w = (*b.offset(
                      i as (isize)
                  ) & (1u32 << (n & 31usize)).wrapping_sub(1u32)) >> nb;
        }
        (if w != 0u32 {
             (ntz(w) as (usize)).wrapping_add(n0) as (u32)
         } else if i == nw.wrapping_sub(1u32) {
             n as (u32)
         } else {
             i = i.wrapping_add(1u32);
             'loop7: loop {
                 if !(i < nw.wrapping_sub(1u32)) {
                     _currentBlock = 8;
                     break;
                 }
                 w = *b.offset(i as (isize));
                 if w != 0u32 {
                     _currentBlock = 15;
                     break;
                 }
                 i = i.wrapping_add(1u32);
             }
             (if _currentBlock == 8 {
                  w = *b.offset(i as (isize));
                  nb = (n & 31usize) as (u32);
                  i = ntz(w) as (u32);
                  (if nb == 0u32 {
                       (i as (usize)).wrapping_add(n.wrapping_sub(32usize)) as (u32)
                   } else if i >= nb {
                       n as (u32)
                   } else {
                       (i as (usize)).wrapping_add(n.wrapping_sub(nb as (usize))) as (u32)
                   })
              } else {
                  (ntz(w) as (u32)).wrapping_add(i << 5i32)
              })
         })
    }
}
