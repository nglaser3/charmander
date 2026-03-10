import h5py
import numpy as np
from pathlib import Path

out = Path("BadFakeU235.h5")
with h5py.File(out, "w") as f:
    # this file is for tricking hdf5 initially, then failing on read
    e = f.require_group("/BadFakeU235/energy")
    e.create_dataset("294K", data=np.array([b"a", b"b", b"c"]))
    x = f.require_group("/BadFakeU235/reactions/reaction_002/294K")
    x.create_dataset("xs", data=np.array([b"a", b"b", b"c"]))

out = Path("FakeU235.h5")
with h5py.File(out, "w") as f:
    e = f.require_group("/FakeU235/energy")
    e.create_dataset("294K", data=np.array([0.0, 1.0, 2.0], dtype=np.float64))
    xs_data = np.array([1.0, 2.0, 3.0], dtype=np.float64)
    dataset_str = "/FakeU235/reactions/reaction_{}/294K"
    for mt in ["002", "004", "018", "102"]:
        f.require_group(dataset_str.format(mt)).create_dataset("xs", data=xs_data)