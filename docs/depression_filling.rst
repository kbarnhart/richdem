Depression-Filling
==================

Depressions, otherwise known as pits, are areas of a landscape wherein flow
ultimately terminates without reaching an ocean or the edge of a digital
elevation model.



Depressions, Pits, and Sinks
----------------------------

Depressions have been called by a variety of names. To clarify this mess,
Lindsay (2016) provides a typology. This typology is followed here.

.. image:: imgs/Lindsay2016_depression_typology.png



Example Dataset
----------------------------

Depression-filling is demonstrated using Beauford Watershed, Minnesota, USA.

.. code-block:: python

    import richdem as rd
    beau    = rd.LoadGDAL("/home/rick/data/gis/beauford.tif")
    beaufig = rd.rdShow(beau, ignore_colours=[0], axes=False, cmap='jet')

.. image:: imgs/beauford.png



Complete Filling
----------------------------

Depression-filling is often used to fill in all the depressions in a DEM to the
level of their lowest outlet or spill-point.

The result looks as follows:

.. code-block:: python

    beau_filled    = rd.FillDepressions(beau, in_place=False)
    beaufig_filled = rd.rdShow(beau_filled, ignore_colours=[0], axes=False, cmap='jet', vmin=beaufig['vmin'], vmax=beaufig['vmax'])

.. image:: imgs/beauford_filled.png

We can visualize the difference between the two like so:

.. code-block:: python

    beau_diff    = beau_filled - beau
    beaufig_diff = rd.rdShow(beau_diff, ignore_colours=[0], axes=False, cmap='jet')

.. image:: imgs/beauford_diff.png

Complete Filling is available via the following commands:

================= ==============================
Language          Command
================= ==============================
Python            `richdem.FillDepressions(dem)`
C++               `richdem::FillDepression(dem)`
================= ==============================

+----------------+--------------------------------------+
|Pros            |  Cons                                |
+----------------+--------------------------------------+
| - *Fast*       | - Leaves flat regions behind         |
| - Simple       | - May modify large portions of a DEM |
+----------------+--------------------------------------+


Epsilon Filling
----------------------------

A downside of complete filling is that it replaces depressions with a perfectly
flat region with no local gradients. One way to deal with this is to ensure that
every cell in the region is raised some small amount, ε, above cells which are
closer to a depression's spill point.

This must be done carefully. In floating-point DEMs, the value ε is non-constant
and must be chosen using the `std::nextafter` function. If a depression is too
large, the imposed gradient may result in the interior of the depression being
raised above the surrounding landscape. Using `double` instead of `float`
reduces the potential for problems at a cost of twice the space used. If a
problem does arise, RichDEM provides a warning.

.. code-block:: python

    beau_epsilon         = rd.FillDepressions(beau, epsilon=True, in_place=False)
    beaufig_epsilon      = rd.rdShow(beau_epsilon, ignore_colours=[0], axes=False, cmap='jet', vmin=beaufig['vmin'], vmax=beaufig['vmax'])
    beau_eps_diff        = beau_epsilon - beau
    beaufig_eps_diff     = rd.rdShow(beau_eps_diff, ignore_colours=[0], axes=False, cmap='jet')
    beau_diffeps_diff    = beau_epsilon - beau_filled
    beaufig_diffeps_diff = rd.rdShow(beau_diffeps_diff, ignore_colours=[0], axes=False, cmap='jet')

.. image:: imgs/beauford_diffeps_diff.png

================= ============================================
Language          Command
================= ============================================
Python            `richdem.FillDepressions(dem, epsilon=True)`
C++               `richdem::FillDepressionsEpsilon(dem)`
================= ============================================

+-------------------+--------------------------------------------+
|Pros               | Cons                                       |
+-------------------+--------------------------------------------+
| - All cells drain | - Not as fast as simple depression filling |
|                   | - May modify large portions of a DEM       |
|                   | - May create elevated regions              |
|                   | - Success may depend on data type          |
+-------------------+--------------------------------------------+