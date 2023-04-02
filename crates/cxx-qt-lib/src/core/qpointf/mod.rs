#[cfg(qt_coord_type = "float")]
mod qpointf32;
#[cfg(not(qt_coord_type = "float"))]
mod qpointf64;

#[cfg(qt_coord_type = "float")]
pub use qpointf32::*;
#[cfg(not(qt_coord_type = "float"))]
pub use qpointf64::*;
