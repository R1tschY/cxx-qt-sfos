#[cfg(qt_coord_type = "float")]
mod qrectf32;
#[cfg(not(qt_coord_type = "float"))]
mod qrectf64;

#[cfg(qt_coord_type = "float")]
pub use qrectf32::*;
#[cfg(not(qt_coord_type = "float"))]
pub use qrectf64::*;
