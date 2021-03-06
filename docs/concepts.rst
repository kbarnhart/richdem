Concepts
===================================

NoData values
-----------------------------------

RichDEM recognizes cells with a *NoData* value and treats them in special ways.
The *NoData* value is a number, such as `-9999` which represents a cell that
isn't part of a data set.

In depression-filling and the determination of flow directions, *NoData* cells
are treated as having a lower elevation than any other cell, which enforces
drainage to the edge of the DEM.

Because *NoData* values fundamental affect operations, RichDEM requires that you
specify what *NoData* value it should use.

It is important that you choose a value that doesn't correspond to any of your
actual data values. In 1-byte/8-bit DEMs this may not be possible since there
are only 256 distinct values available. In this case, you should cast your data
to a 2-byte/16-bit form and choose a *NoData* value that is not in the range
`0-255`.

Other terrain analysis programs may use a binary masking array to indicate which
cells are *NoData*. RichDEM does not do this because usually a minority of cells
are *NoData* and a separate array increases the amount of memory used and
reduces cache utilization, both of which may reduce performance.

================= =================== ========================
Language          Set                 Get
================= =================== ========================
Python            `rda.no_data=-1`    `rda.no_data`
C++               `rda.setNoData(-1)` `rda.noData( )`
================= =================== ========================



Processing History
-----------------------------------

RichDEM automagically keeps track of what operations are performed on your data.
This means that your outputs will contain an exact record of what operations
were used to produce them. This helps ensure reproducibility. And, remember,
good science is reproducible science.

In the following, a series of operations is performed and the Processing History
is then examined.

.. code-block:: python

    import richdem as rd
    dem   = rd.LoadGDAL("/home/rick/data/gis/beauford.tif")
    rd.FillDepressions(dem, epsilon=False, in_place=True)
    accum = rd.FlowAccumulation(dem, method='D8')
    rd.SaveGDAL('/z/out.tif', accum)
    print(accum.metadata['PROCESSING_HISTORY'])

The processing history of a saved dataset can be viewed using a few different
commands:

.. code-block:: bash

    gdalinfo /z/out.tif
    rd_info  /z/out.tif

The processing history appears as follows.

.. code-block:: text

    2017-12-20 17:55:19.892388 UTC | RichDEM (Python 0.0.4) (hash=e02d5e2, hashdate=2017-12-19 23:52:52 -0600) | LoadGDAL(filename=/home/rick/data/gis/beauford.tif, no_data=-9999.0)
    2017-12-20 17:55:19.900234 UTC | RichDEM (Python 0.0.4) (hash=e02d5e2, hashdate=2017-12-19 23:52:52 -0600) | FillDepressions(dem, epsilon=False)
    2017-12-20 17:55:20.514098 UTC | RichDEM (Python 0.0.4) (hash=e02d5e2, hashdate=2017-12-19 23:52:52 -0600) | FlowAccumulation(dem, method=D8)

Note that the **first column** is the time at which the operation was performed,
the **second column** is the program which performed the operation, and the
**third column** is the command which was run.

================= ==============================
Language          Command
================= ==============================
Python            `rda.metadata`
C++               `rda.metadata`
================= ==============================


In-Place Operations
-----------------------------------

To save memory RichDEM performs some operations, such as depression-filling, in
place. This means that the data is modified and the original data will be lost
unless it has been copied.

For instance, in Python `FillDepressions` has two distinct forms:

.. code-block:: python

    #In-place filling, no return value
    rd.FillDepressions(dem, in_place=True)
    #Fill a copy
    dem_filled = rd.FillDepressions(dem, in_place=False)

whereas in C++, a copy must be made:

.. code-block:: python

    #In-place filling, no return value
    richdem::FillDepressions(dem)

    #Fill a copy
    auto demcopy = dem; //TODO: Make sure this syntax is right
    richdem::FillDepressions(demcopy)
