#ifndef LDR_XDR_H
#define LDR_XDR_H

#include "loader.h"
#include "be_mmio.h"

struct xdr_pair {
    unsigned int x;
    unsigned int y;
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

class xdr {
public:
    xdr();
    ~xdr();
    void error(const char *msg);
    void info(const char *msg);
    void fill_stars(char *buf, int n);
    long wake_from_str();
    long enable_scrubbing();
    long print_banner();
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
    long set_config(memory_config *cfg);
    long initialize();
    void dump(int which);
    void dump_xdr();
    void dump_xio();
    void dump_mic();
    void dump_regs();
    long prepare_iow_dump();
    long serial_read(unsigned int ch, unsigned short dev, unsigned char reg,
                     unsigned short *out);

    memory_config *m_mcp;
    xdr_win m_win[2];
    be_mmio m_mic;
    unsigned int tables[6][2][4][9];
    xdr_rec m_recs[16];
};

#endif
