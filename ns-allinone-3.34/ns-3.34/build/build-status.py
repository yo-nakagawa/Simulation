#! /usr/bin/env python3

# Programs that are runnable.
ns3_runnable_programs = ['build/src/aodv/examples/ns3.34-aodv-debug', 'build/src/applications/examples/ns3.34-three-gpp-http-example-debug', 'build/src/bridge/examples/ns3.34-csma-bridge-debug', 'build/src/bridge/examples/ns3.34-csma-bridge-one-hop-debug', 'build/src/buildings/examples/ns3.34-buildings-pathloss-profiler-debug', 'build/src/buildings/examples/ns3.34-outdoor-random-walk-example-debug', 'build/src/config-store/examples/ns3.34-config-store-save-debug', 'build/src/core/examples/ns3.34-main-callback-debug', 'build/src/core/examples/ns3.34-sample-simulator-debug', 'build/src/core/examples/ns3.34-main-ptr-debug', 'build/src/core/examples/ns3.34-main-random-variable-stream-debug', 'build/src/core/examples/ns3.34-sample-random-variable-debug', 'build/src/core/examples/ns3.34-sample-random-variable-stream-debug', 'build/src/core/examples/ns3.34-command-line-example-debug', 'build/src/core/examples/ns3.34-hash-example-debug', 'build/src/core/examples/ns3.34-sample-log-time-format-debug', 'build/src/core/examples/ns3.34-test-string-value-formatting-debug', 'build/src/core/examples/ns3.34-sample-show-progress-debug', 'build/src/core/examples/ns3.34-empirical-random-variable-example-debug', 'build/src/core/examples/ns3.34-system-path-examples-debug', 'build/src/core/examples/ns3.34-fatal-example-debug', 'build/src/core/examples/ns3.34-main-test-sync-debug', 'build/src/core/examples/ns3.34-length-example-debug', 'build/src/csma/examples/ns3.34-csma-one-subnet-debug', 'build/src/csma/examples/ns3.34-csma-broadcast-debug', 'build/src/csma/examples/ns3.34-csma-packet-socket-debug', 'build/src/csma/examples/ns3.34-csma-multicast-debug', 'build/src/csma/examples/ns3.34-csma-raw-ip-socket-debug', 'build/src/csma/examples/ns3.34-csma-ping-debug', 'build/src/csma-layout/examples/ns3.34-csma-star-debug', 'build/src/dsdv/examples/ns3.34-dsdv-manet-debug', 'build/src/dsr/examples/ns3.34-dsr-debug', 'build/src/energy/examples/ns3.34-li-ion-energy-source-debug', 'build/src/energy/examples/ns3.34-rv-battery-model-test-debug', 'build/src/energy/examples/ns3.34-basic-energy-model-test-debug', 'build/src/fd-net-device/examples/ns3.34-dummy-network-debug', 'build/src/fd-net-device/examples/ns3.34-fd2fd-onoff-debug', 'build/src/fd-net-device/examples/ns3.34-realtime-dummy-network-debug', 'build/src/fd-net-device/examples/ns3.34-realtime-fd2fd-onoff-debug', 'build/src/fd-net-device/examples/ns3.34-fd-emu-ping-debug', 'build/src/fd-net-device/examples/ns3.34-fd-emu-onoff-debug', 'build/src/fd-net-device/examples/ns3.34-fd-emu-tc-debug', 'build/src/fd-net-device/examples/ns3.34-fd-emu-send-debug', 'build/src/fd-net-device/examples/ns3.34-fd-emu-udp-echo-debug', 'build/src/fd-net-device/examples/ns3.34-fd-tap-ping-debug', 'build/src/fd-net-device/examples/ns3.34-fd-tap-ping6-debug', 'build/src/internet/examples/ns3.34-main-simple-debug', 'build/src/internet-apps/examples/ns3.34-dhcp-example-debug', 'build/src/internet-apps/examples/ns3.34-traceroute-example-debug', 'build/src/lr-wpan/examples/ns3.34-lr-wpan-packet-print-debug', 'build/src/lr-wpan/examples/ns3.34-lr-wpan-phy-test-debug', 'build/src/lr-wpan/examples/ns3.34-lr-wpan-data-debug', 'build/src/lr-wpan/examples/ns3.34-lr-wpan-error-model-plot-debug', 'build/src/lr-wpan/examples/ns3.34-lr-wpan-error-distance-plot-debug', 'build/src/lr-wpan/examples/ns3.34-lr-wpan-mlme-debug', 'build/src/lte/examples/ns3.34-lena-cqi-threshold-debug', 'build/src/lte/examples/ns3.34-lena-dual-stripe-debug', 'build/src/lte/examples/ns3.34-lena-fading-debug', 'build/src/lte/examples/ns3.34-lena-intercell-interference-debug', 'build/src/lte/examples/ns3.34-lena-ipv6-addr-conf-debug', 'build/src/lte/examples/ns3.34-lena-ipv6-ue-rh-debug', 'build/src/lte/examples/ns3.34-lena-ipv6-ue-ue-debug', 'build/src/lte/examples/ns3.34-lena-pathloss-traces-debug', 'build/src/lte/examples/ns3.34-lena-profiling-debug', 'build/src/lte/examples/ns3.34-lena-rem-debug', 'build/src/lte/examples/ns3.34-lena-rem-sector-antenna-debug', 'build/src/lte/examples/ns3.34-lena-rlc-traces-debug', 'build/src/lte/examples/ns3.34-lena-simple-debug', 'build/src/lte/examples/ns3.34-lena-simple-epc-debug', 'build/src/lte/examples/ns3.34-lena-simple-epc-backhaul-debug', 'build/src/lte/examples/ns3.34-lena-deactivate-bearer-debug', 'build/src/lte/examples/ns3.34-lena-x2-handover-debug', 'build/src/lte/examples/ns3.34-lena-x2-handover-measures-debug', 'build/src/lte/examples/ns3.34-lena-frequency-reuse-debug', 'build/src/lte/examples/ns3.34-lena-distributed-ffr-debug', 'build/src/lte/examples/ns3.34-lena-uplink-power-control-debug', 'build/src/lte/examples/ns3.34-lena-radio-link-failure-debug', 'build/src/lte/examples/ns3.34-lena-simple-epc-emu-debug', 'build/src/mesh/examples/ns3.34-mesh-debug', 'build/src/mobility/examples/ns3.34-main-grid-topology-debug', 'build/src/mobility/examples/ns3.34-main-random-topology-debug', 'build/src/mobility/examples/ns3.34-main-random-walk-debug', 'build/src/mobility/examples/ns3.34-mobility-trace-example-debug', 'build/src/mobility/examples/ns3.34-ns2-mobility-trace-debug', 'build/src/mobility/examples/ns3.34-bonnmotion-ns2-example-debug', 'build/src/netanim/examples/ns3.34-dumbbell-animation-debug', 'build/src/netanim/examples/ns3.34-grid-animation-debug', 'build/src/netanim/examples/ns3.34-star-animation-debug', 'build/src/netanim/examples/ns3.34-wireless-animation-debug', 'build/src/netanim/examples/ns3.34-uan-animation-debug', 'build/src/netanim/examples/ns3.34-colors-link-description-debug', 'build/src/netanim/examples/ns3.34-resources-counters-debug', 'build/src/network/examples/ns3.34-main-packet-header-debug', 'build/src/network/examples/ns3.34-main-packet-tag-debug', 'build/src/network/examples/ns3.34-packet-socket-apps-debug', 'build/src/network/examples/ns3.34-lollipop-comparisions-debug', 'build/src/network/examples/ns3.34-bit-serializer-debug', 'build/src/nix-vector-routing/examples/ns3.34-nix-simple-debug', 'build/src/nix-vector-routing/examples/ns3.34-nms-p2p-nix-debug', 'build/src/nix-vector-routing/examples/ns3.34-nix-simple-multi-address-debug', 'build/src/olsr/examples/ns3.34-simple-point-to-point-olsr-debug', 'build/src/olsr/examples/ns3.34-olsr-hna-debug', 'build/src/point-to-point/examples/ns3.34-main-attribute-value-debug', 'build/src/propagation/examples/ns3.34-main-propagation-loss-debug', 'build/src/propagation/examples/ns3.34-jakes-propagation-model-example-debug', 'build/src/sixlowpan/examples/ns3.34-example-sixlowpan-debug', 'build/src/sixlowpan/examples/ns3.34-example-ping-lr-wpan-debug', 'build/src/sixlowpan/examples/ns3.34-example-ping-lr-wpan-beacon-debug', 'build/src/sixlowpan/examples/ns3.34-example-ping-lr-wpan-mesh-under-debug', 'build/src/spectrum/examples/ns3.34-adhoc-aloha-ideal-phy-debug', 'build/src/spectrum/examples/ns3.34-adhoc-aloha-ideal-phy-matrix-propagation-loss-model-debug', 'build/src/spectrum/examples/ns3.34-adhoc-aloha-ideal-phy-with-microwave-oven-debug', 'build/src/spectrum/examples/ns3.34-tv-trans-example-debug', 'build/src/spectrum/examples/ns3.34-tv-trans-regional-example-debug', 'build/src/spectrum/examples/ns3.34-three-gpp-channel-example-debug', 'build/src/stats/examples/ns3.34-gnuplot-example-debug', 'build/src/stats/examples/ns3.34-double-probe-example-debug', 'build/src/stats/examples/ns3.34-time-probe-example-debug', 'build/src/stats/examples/ns3.34-gnuplot-aggregator-example-debug', 'build/src/stats/examples/ns3.34-gnuplot-helper-example-debug', 'build/src/stats/examples/ns3.34-file-aggregator-example-debug', 'build/src/stats/examples/ns3.34-file-helper-example-debug', 'build/src/tap-bridge/examples/ns3.34-tap-csma-debug', 'build/src/tap-bridge/examples/ns3.34-tap-csma-virtual-machine-debug', 'build/src/tap-bridge/examples/ns3.34-tap-wifi-virtual-machine-debug', 'build/src/tap-bridge/examples/ns3.34-tap-wifi-dumbbell-debug', 'build/src/topology-read/examples/ns3.34-topology-example-sim-debug', 'build/src/traffic-control/examples/ns3.34-red-tests-debug', 'build/src/traffic-control/examples/ns3.34-red-vs-ared-debug', 'build/src/traffic-control/examples/ns3.34-adaptive-red-tests-debug', 'build/src/traffic-control/examples/ns3.34-pfifo-vs-red-debug', 'build/src/traffic-control/examples/ns3.34-codel-vs-pfifo-basic-test-debug', 'build/src/traffic-control/examples/ns3.34-codel-vs-pfifo-asymmetric-debug', 'build/src/traffic-control/examples/ns3.34-pie-example-debug', 'build/src/traffic-control/examples/ns3.34-fqcodel-l4s-example-debug', 'build/src/uan/examples/ns3.34-uan-cw-example-debug', 'build/src/uan/examples/ns3.34-uan-rc-example-debug', 'build/src/uan/examples/ns3.34-uan-raw-example-debug', 'build/src/uan/examples/ns3.34-uan-ipv4-example-debug', 'build/src/uan/examples/ns3.34-uan-ipv6-example-debug', 'build/src/uan/examples/ns3.34-uan-6lowpan-example-debug', 'build/src/virtual-net-device/examples/ns3.34-virtual-net-device-debug', 'build/src/wave/examples/ns3.34-wave-simple-80211p-debug', 'build/src/wave/examples/ns3.34-wave-simple-device-debug', 'build/src/wave/examples/ns3.34-vanet-routing-compare-debug', 'build/src/wifi/examples/ns3.34-wifi-phy-test-debug', 'build/src/wifi/examples/ns3.34-wifi-test-interference-helper-debug', 'build/src/wifi/examples/ns3.34-wifi-manager-example-debug', 'build/src/wifi/examples/ns3.34-wifi-trans-example-debug', 'build/src/wifi/examples/ns3.34-wifi-phy-configuration-debug', 'build/src/wifi/examples/ns3.34-wifi-bianchi-debug', 'build/src/wimax/examples/ns3.34-wimax-ipv4-debug', 'build/src/wimax/examples/ns3.34-wimax-multicast-debug', 'build/src/wimax/examples/ns3.34-wimax-simple-debug', 'build/examples/ipv6/ns3.34-icmpv6-redirect-debug', 'build/examples/ipv6/ns3.34-ping6-debug', 'build/examples/ipv6/ns3.34-radvd-debug', 'build/examples/ipv6/ns3.34-radvd-two-prefix-debug', 'build/examples/ipv6/ns3.34-test-ipv6-debug', 'build/examples/ipv6/ns3.34-fragmentation-ipv6-debug', 'build/examples/ipv6/ns3.34-fragmentation-ipv6-two-MTU-debug', 'build/examples/ipv6/ns3.34-loose-routing-ipv6-debug', 'build/examples/ipv6/ns3.34-wsn-ping6-debug', 'build/examples/tutorial/ns3.34-hello-simulator-debug', 'build/examples/tutorial/ns3.34-first-debug', 'build/examples/tutorial/ns3.34-second-debug', 'build/examples/tutorial/ns3.34-third-debug', 'build/examples/tutorial/ns3.34-fourth-debug', 'build/examples/tutorial/ns3.34-fifth-debug', 'build/examples/tutorial/ns3.34-sixth-debug', 'build/examples/tutorial/ns3.34-seventh-debug', 'build/examples/routing/ns3.34-dynamic-global-routing-debug', 'build/examples/routing/ns3.34-static-routing-slash32-debug', 'build/examples/routing/ns3.34-global-routing-slash32-debug', 'build/examples/routing/ns3.34-global-injection-slash32-debug', 'build/examples/routing/ns3.34-simple-global-routing-debug', 'build/examples/routing/ns3.34-simple-alternate-routing-debug', 'build/examples/routing/ns3.34-mixed-global-routing-debug', 'build/examples/routing/ns3.34-simple-routing-ping6-debug', 'build/examples/routing/ns3.34-manet-routing-compare-debug', 'build/examples/routing/ns3.34-ripng-simple-network-debug', 'build/examples/routing/ns3.34-rip-simple-network-debug', 'build/examples/routing/ns3.34-global-routing-multi-switch-plus-router-debug', 'build/examples/routing/ns3.34-simple-multicast-flooding-debug', 'build/examples/tcp/ns3.34-tcp-large-transfer-debug', 'build/examples/tcp/ns3.34-tcp-nsc-lfn-debug', 'build/examples/tcp/ns3.34-tcp-nsc-zoo-debug', 'build/examples/tcp/ns3.34-tcp-star-server-debug', 'build/examples/tcp/ns3.34-star-debug', 'build/examples/tcp/ns3.34-tcp-bulk-send-debug', 'build/examples/tcp/ns3.34-tcp-pcap-nanosec-example-debug', 'build/examples/tcp/ns3.34-tcp-nsc-comparison-debug', 'build/examples/tcp/ns3.34-tcp-variants-comparison-debug', 'build/examples/tcp/ns3.34-tcp-pacing-debug', 'build/examples/tcp/ns3.34-dctcp-example-debug', 'build/examples/tcp/ns3.34-tcp-linux-reno-debug', 'build/examples/tcp/ns3.34-tcp-validation-debug', 'build/examples/tcp/ns3.34-tcp-bbr-example-debug', 'build/examples/traffic-control/ns3.34-traffic-control-debug', 'build/examples/traffic-control/ns3.34-queue-discs-benchmark-debug', 'build/examples/traffic-control/ns3.34-red-vs-fengadaptive-debug', 'build/examples/traffic-control/ns3.34-red-vs-nlred-debug', 'build/examples/traffic-control/ns3.34-tbf-example-debug', 'build/examples/traffic-control/ns3.34-cobalt-vs-codel-debug', 'build/examples/energy/ns3.34-energy-model-example-debug', 'build/examples/energy/ns3.34-energy-model-with-harvesting-example-debug', 'build/examples/matrix-topology/ns3.34-matrix-topology-debug', 'build/examples/realtime/ns3.34-realtime-udp-echo-debug', 'build/examples/udp/ns3.34-udp-echo-debug', 'build/examples/wireless/ns3.34-mixed-wired-wireless-debug', 'build/examples/wireless/ns3.34-wifi-adhoc-debug', 'build/examples/wireless/ns3.34-wifi-clear-channel-cmu-debug', 'build/examples/wireless/ns3.34-wifi-ap-debug', 'build/examples/wireless/ns3.34-wifi-wired-bridging-debug', 'build/examples/wireless/ns3.34-wifi-multirate-debug', 'build/examples/wireless/ns3.34-wifi-simple-adhoc-debug', 'build/examples/wireless/ns3.34-wifi-simple-adhoc-grid-debug', 'build/examples/wireless/ns3.34-wifi-simple-infra-debug', 'build/examples/wireless/ns3.34-wifi-simple-interference-debug', 'build/examples/wireless/ns3.34-wifi-blockack-debug', 'build/examples/wireless/ns3.34-wifi-dsss-validation-debug', 'build/examples/wireless/ns3.34-wifi-ofdm-validation-debug', 'build/examples/wireless/ns3.34-wifi-ofdm-ht-validation-debug', 'build/examples/wireless/ns3.34-wifi-ofdm-vht-validation-debug', 'build/examples/wireless/ns3.34-wifi-hidden-terminal-debug', 'build/examples/wireless/ns3.34-wifi-ht-network-debug', 'build/examples/wireless/ns3.34-wifi-vht-network-debug', 'build/examples/wireless/ns3.34-wifi-timing-attributes-debug', 'build/examples/wireless/ns3.34-wifi-sleep-debug', 'build/examples/wireless/ns3.34-wifi-power-adaptation-distance-debug', 'build/examples/wireless/ns3.34-wifi-power-adaptation-interference-debug', 'build/examples/wireless/ns3.34-wifi-rate-adaptation-distance-debug', 'build/examples/wireless/ns3.34-wifi-aggregation-debug', 'build/examples/wireless/ns3.34-wifi-txop-aggregation-debug', 'build/examples/wireless/ns3.34-wifi-simple-ht-hidden-stations-debug', 'build/examples/wireless/ns3.34-wifi-80211n-mimo-debug', 'build/examples/wireless/ns3.34-wifi-mixed-network-debug', 'build/examples/wireless/ns3.34-wifi-tcp-debug', 'build/examples/wireless/ns3.34-wifi-80211e-txop-debug', 'build/examples/wireless/ns3.34-wifi-spectrum-per-example-debug', 'build/examples/wireless/ns3.34-wifi-spectrum-per-interference-debug', 'build/examples/wireless/ns3.34-wifi-spectrum-saturation-example-debug', 'build/examples/wireless/ns3.34-wifi-ofdm-he-validation-debug', 'build/examples/wireless/ns3.34-wifi-he-network-debug', 'build/examples/wireless/ns3.34-wifi-multi-tos-debug', 'build/examples/wireless/ns3.34-wifi-backward-compatibility-debug', 'build/examples/wireless/ns3.34-wifi-spatial-reuse-debug', 'build/examples/wireless/ns3.34-wifi-error-models-comparison-debug', 'build/examples/socket/ns3.34-socket-bound-static-routing-debug', 'build/examples/socket/ns3.34-socket-bound-tcp-static-routing-debug', 'build/examples/socket/ns3.34-socket-options-ipv4-debug', 'build/examples/socket/ns3.34-socket-options-ipv6-debug', 'build/examples/stats/ns3.34-wifi-example-sim-debug', 'build/examples/error-model/ns3.34-simple-error-model-debug', 'build/examples/naming/ns3.34-object-names-debug', 'build/examples/channel-models/ns3.34-three-gpp-v2v-channel-example-debug', 'build/examples/udp-client-server/ns3.34-udp-client-server-debug', 'build/examples/udp-client-server/ns3.34-udp-trace-client-server-debug', 'build/scratch/ns3.34-scratch-simulator-debug', 'build/scratch/subdir/ns3.34-subdir-debug']

# Scripts that are runnable.
ns3_runnable_scripts = ['csma-bridge.py', 'sample-simulator.py', 'wifi-olsr-flowmon.py', 'tap-csma-virtual-machine.py', 'tap-wifi-virtual-machine.py', 'first.py', 'second.py', 'third.py', 'simple-routing-ping6.py', 'realtime-udp-echo.py', 'mixed-wired-wireless.py', 'wifi-ap.py']

