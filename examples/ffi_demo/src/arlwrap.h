// Author: Bojan Nikolic <b.nikolic@mrao.cam.ac.uk>
// Header for C-Wrapped version of ARL
//
#ifndef __ARLWRAP_H__
#define __ARLWRAP_H__

#include <complex.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  size_t nvis;
  int npol;
  // This needs to be interpret differently dependent on the value of
  // the npol. For example when npol is 4, data is equivalent to "C"
  // type type "ARLVisEntryP4[nvis]"
  void *data;
  char *phasecentre;
} ARLVis;

// This is what the data array for four polarisations look like. Can
// recast data to this (or equivalent of 2 or 1 polarisation if really
// needed. This memory layout show allow re-use in numpy without any
// data copying.
typedef struct {
  double uvw[3];
  double time;
  double freq;
  double bw;
  double intgt;
  int a1;
  int a2;
  float complex vis[4];
  float wght[4];
  float imgwght [4];
} ARLVisEntryP4;

  void arl_copy_visibility(const ARLVis *visin,
			   ARLVis *visout,
			   bool zero);

// A structure to store the GainTable
typedef struct {
  size_t nrows;
  void *data;
} ARLGt;

typedef struct {
	size_t size;
	int data_shape[4];
	void *data;
	char *wcs;
	char *polarisation_frame;
} Image;

typedef struct {
  char *confname;
  double pc_ra;
  double pc_dec;
  double *times;
  int ntimes;
  double *freqs;
  int nfreqs;
  double *channel_bandwidth;
  int nchanwidth;
  int nbases;
  int nant;
  int npol;
  int nrec;
  double rmax;
  char *polframe;
} ARLConf;

typedef struct {int nant, nbases;} ant_t;

typedef struct {
	int vis_slices;
	int npixel;
	double cellsize;
	double guard_band_image;
	double delA;
	int wprojection_planes;
	} ARLadvice;


// Prototypes to ARL routines
void helper_get_image_shape(const double *frequency, double cellsize,
		int *shape);

void helper_get_image_shape_multifreq(ARLConf *lowconf, double cellsize,
		int npixel, int *shape);

void helper_get_nbases(char *, ant_t *);
void helper_set_image_params(const ARLVis *vis, Image *image);

void arl_create_visibility(ARLConf *lowconf, ARLVis *res_vis);
void arl_create_blockvisibility(ARLConf *lowconf, ARLVis *res_vis);
void arl_advise_wide_field(ARLConf *lowconf, ARLVis *res_vis, ARLadvice * adv);

void arl_create_test_image(const double *frequency, double cellsize, char *phasecentre,
		Image *res_img);
void arl_create_low_test_image_from_gleam(ARLConf *lowconf, double cellsize, int npixel, char *phasecentre,
		Image *res_img);


void arl_predict_2d(const ARLVis *visin, const Image *img, ARLVis *visout);
void arl_invert_2d(const ARLVis *visin, const Image *img_in, bool dopsf, Image *out, double *sumwt);

void arl_create_image_from_visibility(const ARLVis *vis, Image *model);
void arl_create_image_from_blockvisibility(ARLConf *lowconf, const ARLVis *blockvis, double cellsize, int npixel, char* phasecentre, Image *model);
void arl_deconvolve_cube(Image *dirty, Image *psf, Image *restored,
		Image *residual);
void arl_restore_cube(Image *model, Image *psf, Image *residual,
		Image *restored);

void arl_predict_function(ARLConf *lowconf, const ARLVis *visin, const Image *img, ARLVis *visout, ARLVis *blockvisout, long long int *cindexout);
void arl_invert_function(ARLConf * lowconf, const ARLVis *visin, Image * img_model, int vis_slices, Image * img_dirty);
void arl_ical(ARLConf * lowconf, const ARLVis *visin, Image * img_model, int vis_slices, Image * img_deconv, Image * img_resid, Image * img_rest);
void arl_convert_visibility_to_blockvisibility(ARLConf *lowconf, const ARLVis *visin, const ARLVis *blockvisin, long long int *cindexin, ARLVis *visout);
void arl_create_gaintable_from_blockvisibility(ARLConf *lowconf, const ARLVis *visin, ARLGt *gtout);
void arl_predict_function_blockvis(ARLConf *, ARLVis *, const Image *);
void arl_invert_function(ARLConf *, const ARLVis *, Image *, int, Image *);

void arl_simulate_gaintable(ARLConf *, ARLGt *gt);
void arl_apply_gaintable(ARLConf *lowconf, const ARLVis *visin, ARLGt *gtin, ARLVis *visout);

/** Initialise the ARL library
 */
void arl_initialize(void);

#ifdef __cplusplus
}
#endif

#endif
