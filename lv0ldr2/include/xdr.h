#ifndef LDR_XDR_H
#define LDR_XDR_H

#include "loader.h"
#include "be_mmio.h"

struct xdr_pair {
    unsigned char x;
    unsigned char y;
};

struct xdr_win {
    unsigned short rx_dq;
    unsigned short rx_pin;
    unsigned short rx_ui;
    unsigned short tx_dq;
    unsigned short tx_pin;
    unsigned short tx_ui;
};

struct xdr_rec {
    unsigned int w0;
    unsigned int w1;
    unsigned char b;
};

struct xdr_config {
    unsigned char basic[128];
    unsigned char str;
    unsigned int f_84;
    unsigned int cbe_kind;
    unsigned int f_8c;
    unsigned short f_ac;
    unsigned int f_b0;
    unsigned int f_b4;
    unsigned int f_b8;
    bool ecc_off_ch0;
    bool ecc_off_ch1;
    bool ecc_on;
    bool xio_ch0_on;
    bool xio_ch1_on;
};

class xdr {
public:
    xdr();
    void error(const char *msg);
    void info(const char *msg);
    void fill_stars(char *buf, int n);
    long wake_from_str();
    long enable_scrubbing();
    long validate_config();
    long begin();
    long load_rows(unsigned short *dst);
    unsigned short bank_map(int i, unsigned int ratio);
    xdr_pair timing(int a, int b, int c);
    unsigned short permute(unsigned short x);
    long store_zero_line_mode(unsigned int v);
    unsigned int row_address(unsigned int ratio, unsigned int v);
    long pack(int idx, const unsigned short *rows, xdr_rec *out);
    void write_rows(unsigned int count, const unsigned short *rows);
    long write_xdr_rows();
    void write_tx_tables();
    void write_rx_tables();
    long check_channel_windows(int ch);
    void add_rx_readings();
    void add_tx_readings();
    void fill_hex2(char *out, unsigned int v);
    void fill_dec2(char *out, unsigned int v);
    long print_window_summary();
    long check_windows();
    long load_rows_to_devices();
    long write_mic_config();
    long init_xio_link();
    long init_devices();
    long calibrate_rx();
    long store_zero_line(unsigned int v);
    long run_mic_cmd_c0();
    long calibrate_tx();
    long read_ptcal_start();
    long write_ptcal_settings();
    long enable_ptcal();
    long set_config(const xdr_config *cfg);
    long initialize();
    void dump(int which);
    void dump_xdr();
    void dump_xio();
    void dump_mic();
    void dump_regs();
    long prepare_iow_dump();
    long serial_read(unsigned int ch, unsigned short dev, unsigned char reg,
                     unsigned short *out);

    xdr_config m_cfg;
    xdr_config *m_mcp;
    unsigned short m_ac;
    unsigned short m_ae;
    unsigned short m_b0;
    unsigned short m_b2;
    unsigned short m_b4;
    unsigned int m_b8;
    unsigned short m_bc[6];
    bool m_c8;
    xdr_win m_win[2];
    be_mmio m_mic;
    unsigned int m_rx_mid[2][4][9];
    unsigned int m_rx_left[2][4][9];
    unsigned int m_rx_right[2][4][9];
    unsigned int m_tx_mid[2][4][9];
    unsigned int m_tx_left[2][4][9];
    unsigned int m_tx_right[2][4][9];
    xdr_rec m_recs[16];
};

namespace sc_config_info {
class xdr {
public:
    void initialize(void);
    long identify_cbe_version_from_spu_vr(unsigned int *kind, unsigned int *level);
    long is_waking_from_suspend_to_ram_mode(unsigned char *str);
    long query_config(xdr_config *cfg);
};
}

#endif
